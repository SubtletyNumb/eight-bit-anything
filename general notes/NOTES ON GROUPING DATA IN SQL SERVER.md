Depending on how you want the result to look (a list of rows, a comma-separated list, or a JSON object), there are three main ways to do this in SQL Server.

Here is the setup I am assuming for these examples:
*   **Table `Types`**: (`TypeID`, `TypeName`)
*   **Table `Groups`**: (`GroupID`, `GroupCode`)
*   **Table `Groups_Types`**: (`TypeID`, `GroupID`)

### Option 1: The "Comma-Separated List" (Best for Reports)
If you want **one row per Type** and a column containing all related Group Codes separated by a comma, use `STRING_AGG` (available in SQL Server 2017 and newer).

```sql
SELECT 
    T.TypeName,
    STRING_AGG(G.GroupCode, ', ') AS RelatedGroups
FROM 
    Types T
JOIN 
    Groups_Types GT ON T.TypeID = GT.TypeID
JOIN 
    Groups G ON GT.GroupID = G.GroupID
GROUP BY 
    T.TypeName;
```

**Result:**
| TypeName | RelatedGroups |
| :--- | :--- |
| Type A | Code1, Code2, Code5 |
| Type B | Code3, Code4 |

---

### Option 2: The "Standard Join" (Best for Filtering)
If you simply want a list of every combination (which means the Type Name will repeat), use a standard `JOIN`.

```sql
SELECT 
    T.TypeName,
    G.GroupCode
FROM 
    Types T
JOIN 
    Groups_Types GT ON T.TypeID = GT.TypeID
JOIN 
    Groups G ON GT.GroupID = G.GroupID
ORDER BY 
    T.TypeName;
```

**Result:**
| TypeName | GroupCode |
| :--- | :--- |
| Type A | Code1 |
| Type A | Code2 |
| Type A | Code5 |
| Type B | Code3 |

---

### Option 3: The "JSON" Approach (Best for APIs/Web Apps)
If you are sending this data to a frontend application (like React, Angular, or a mobile app), SQL Server can format the result as a nested JSON object automatically.

```sql
SELECT 
    T.TypeName,
    (
        SELECT G.GroupCode, G.GroupName
        FROM Groups G
        INNER JOIN Groups_Types GT ON G.GroupID = GT.GroupID
        WHERE GT.TypeID = T.TypeID
        FOR JSON PATH
    ) AS Groups
FROM 
    Types T
FOR JSON PATH;
```

**Result (JSON):**
```json
[
  {
    "TypeName": "Type A",
    "Groups": [
      {"GroupCode": "Code1"},
      {"GroupCode": "Code2"}
    ]
  },
  {
    "TypeName": "Type B",
    "Groups": [
      {"GroupCode": "Code3"}
    ]
  }
]
```

### Which one should I choose?
1.  **Use Option 1** if you want to see a clean list on a screen or in Excel.
2.  **Use Option 2** if you need to perform further logic (like `WHERE GroupCode = 'X'`).
3.  **Use Option 3** if you are building an API or passing data to a web developer.