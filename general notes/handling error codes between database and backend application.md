Handling errors between SQL Server and Node.js requires a "handshake" where SQL handles the technical heavy lifting and Node.js translates that into something a human can actually read.

Here is the industry-standard approach to bridging that gap.

---

## 1. The SQL Server Side: `TRY...CATCH` and `THROW`

Inside your stored procedure, you want to catch the "ugly" system error and re-throw it in a way that Node.js can easily parse. The modern approach is using `THROW` rather than `RAISERROR`.

```sql
CREATE PROCEDURE usp_RegisterUser
    @Email NVARCHAR(255)
AS
BEGIN
    SET NOCOUNT ON;
    BEGIN TRY
        -- Example logic
        INSERT INTO Users (Email) VALUES (@Email);
    END TRY
    BEGIN CATCH
        -- 1. Log the internal error for the DBAs
        -- 2. Throw a custom error number (50000+) for Node.js
        DECLARE @ErrMsg NVARCHAR(4000) = ERROR_MESSAGE();
        DECLARE @ErrNum INT = ERROR_NUMBER();

        IF @ErrNum = 2627 -- Unique Constraint (Email exists)
        BEGIN
            ;THROW 51000, 'This email address is already registered.', 1;
        END
        ELSE
        BEGIN
            -- Generic catch-all for anything else
            ;THROW 50000, 'An unexpected database error occurred.', 1;
        END
    END CATCH
END

```

---

## 2. The Node.js Side: Catching the Exception

When using libraries like `mssql` (tedious), SQL errors are returned as exceptions. You can inspect the `number` property of the error object to decide what to tell the user.

```javascript
const sql = require('mssql');

async function registerUser(email) {
    try {
        let pool = await sql.connect(config);
        await pool.request()
            .input('Email', sql.NVarChar, email)
            .execute('usp_RegisterUser');
    } catch (err) {
        // 'err.number' corresponds to the number in our SQL THROW
        const friendlyMessage = mapSqlError(err);
        return { success: false, message: friendlyMessage };
    }
}

function mapSqlError(err) {
    switch (err.number) {
        case 51000:
            return "Email already in use. Please try logging in.";
        case 547:
            return "This action violates data integrity (Check constraints).";
        case 2627:
            return "That record already exists.";
        default:
            return "Something went wrong on our end. Please try again later.";
    }
}

```

---

## 3. Best Practices for "Friendly" Messages

* **Don't expose the Engine:** Never return `err.message` directly from SQL Server to the frontend. It often contains table names, column names, or server info that is a security risk.
* **Use Custom Ranges:** Use the **50,000+** range for business logic errors (e.g., "Insufficient Funds"). This keeps them separate from actual system crashes.
* **Log the Real Error:** While the user sees "Email already in use," your Node.js server should log the original `err.stack` so you can debug what actually happened.

---

### Summary of the Flow

1. **SQL Server** detects a constraint violation (e.g., Error 2627).
2. **Stored Procedure** catches it and `THROW`s a custom ID (e.g., 51000).
3. **Node.js** catches the exception and checks `if (err.number === 51000)`.
4. **Frontend** receives a clean JSON response: `{ "error": "Email already in use" }`.

Would you like me to show you how to create a centralized Error Mapping table in SQL so you don't have to hardcode these strings in your Node.js logic?


So basically we need to have the common errors like, that record already exists, unique violation, etc, and we also have the custom error codes that we throw from the sql server like the 51000 for email already in use, we can have then a switch case to map those values and then return a user friendly message from that? 


Exactly. You've nailed the architecture. You treat the SQL Error Number as a **unique key** that your Node.js application uses to look up a "human-readable" translation.

This approach is highly effective because it separates **Data Logic** (what happened) from **Presentation Logic** (how to tell the user).

### The "Lookup Table" Strategy

As your app grows, a `switch` statement can get messy. Many developers move this into a **mapping object** or a small utility module.

Here is how that looks in a production-ready pattern:

```javascript
// errorMapper.js
const ErrorMessages = {
    // Standard SQL Engine Errors
    2627: "A record with this information already exists.", // Unique Constraint
    547:  "This item is currently in use and cannot be deleted.", // Foreign Key
    2601: "Duplicate entry found.", // Unique Index
    
    // Your Custom Business Logic Errors (50000+)
    51000: "That email address is already registered to an account.",
    51001: "Your account balance is too low for this transaction.",
    51002: "This promotion code has expired."
};

/**
 * Translates a raw SQL error into a friendly message
 */
function getFriendlyMessage(err) {
    // If the error number exists in our map, return it
    // Otherwise, return a generic "safe" message
    return ErrorMessages[err.number] || "An unexpected error occurred. Please try again.";
}

module.exports = getFriendlyMessage;

```

### Why this is better than `RAISERROR` strings

If you change your mind and want the message to say *"Email already taken"* instead of *"Email address already registered,"* you only change **one line** in your Node.js code. You don't have to:

1. Open the SQL Stored Procedure.
2. Alter the Procedure.
3. Deploy it to Production.

### Pro-Tip: Handling Variables in Messages

Sometimes you want the message to be specific, like: *"The item **'Laptop'** is out of stock."* In those cases, you can pass the specific item name in the `message` parameter of the SQL `THROW` command. In Node.js, you would check if it's a "known" custom error and then just pass `err.message` directly through, since you wrote that message yourself in the Stored Procedure.

---

### A Note on Security

**Never** use a "default" case that returns `err.message` to the user.

* **Bad:** `return err.message;` (Might reveal: *"Invalid column name 'CreditCard_CVV' in Table 'PrivateData'"*)
* **Good:** `return "Internal Server Error";` (Reveals nothing to a hacker).

Would you like to see how to handle **Internationalization (i18n)** with this setup so you can return messages in different languages based on the user's preference?


 i can extend it and instead of returning the message, i can also have like a type or category error type.


Lets say:

errorMessages = {

2627: {type: DUPLICATED, message: `That ${errMessage} already exits` }

} 


This is exactly how professional APIs are built. By returning a **structured object** instead of a flat string, your frontend doesn't have to "guess" what happened by reading text—it can look at the `type` and make logic decisions (like shaking a specific input field or showing a specific icon).

Here is how you can implement that structure in your Node.js layer to handle both static types and dynamic messages from SQL.

### The Structured Error Map

```javascript
const ErrorCategories = {
    UNIQUE_VIOLATION: 'DUPLICATED',
    FOREIGN_KEY: 'REFERENCE_ERROR',
    BUSINESS_RULE: 'VALIDATION_ERROR',
    GENERIC: 'INTERNAL_ERROR'
};

const errorMessages = {
    // System Errors
    2627: { 
        type: ErrorCategories.UNIQUE_VIOLATION, 
        message: "This record already exists in our system." 
    },
    547: { 
        type: ErrorCategories.FOREIGN_KEY, 
        message: "This item cannot be removed because it is linked to other data." 
    },

    // Custom Business Logic (50000+)
    51000: { 
        type: ErrorCategories.BUSINESS_RULE, 
        message: "The email address is already in use." 
    }
};

```

---

### Handling Dynamic Messages (The `${errMessage}` part)

As you noted, sometimes you want the message to be dynamic. Since `THROW` in SQL Server allows you to pass a custom string, you can capture that and inject it into your object.

```javascript
function formatError(err) {
    const lookup = errorMessages[err.number];

    if (lookup) {
        return {
            success: false,
            error: {
                code: err.number,
                type: lookup.type,
                // Use the custom message from SQL if available, otherwise the map
                message: err.message.startsWith('Custom:') 
                         ? err.message.replace('Custom:', '') 
                         : lookup.message
            }
        };
    }

    // Fallback for unmapped engine errors
    return {
        success: false,
        error: {
            code: err.number,
            type: ErrorCategories.GENERIC,
            message: "An unexpected error occurred."
        }
    };
}

```

### Why this "Type" approach wins:

1. **Frontend Logic:** Your React/Vue code can do this:
```javascript
if (response.error.type === 'DUPLICATED') {
    showToast("Double entry detected!", "warning");
}

```


2. **Clean Logs:** You can log the `type` to your monitoring dashboard (like Datadog or Sentry) to see which *category* of errors is hitting your users most often.
3. **Scalability:** If you ever move from SQL Server to PostgreSQL, your Node.js logic stays exactly the same; you just update the error numbers in your map.

---

### Example SQL implementation for dynamic messages:

To make that dynamic message work, your Stored Procedure would look like this:

```sql
BEGIN CATCH
    IF ERROR_NUMBER() = 2627
    BEGIN
        -- We prefix with "Custom:" so Node knows to trust this string
        DECLARE @Msg NVARCHAR(200) = 'Custom:The ' + @EntityName + ' already exists.';
        THROW 51000, @Msg, 1;
    END
END CATCH

```

Would you like me to show you how to wrap this into a **Express.js middleware** so it automatically catches and formats these errors for every single API route?