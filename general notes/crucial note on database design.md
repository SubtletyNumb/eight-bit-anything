This is a crucial design decision. You are dealing with two very different types of relationships here: **Transactional Parents** vs. **Master Data (Catalog) Parents**.

Here is exactly what should happen and how to write the code.

### 1. The "Header" (ArticuloLoteValidacion) -> `ON DELETE CASCADE`
**The Logic:** If you delete the main validation report (the Header), the individual results inside it (the Details) become garbage. They cannot exist without the header.
**The Action:** You want **Cascade Delete**.

### 2. The "Type" (TipoArticuloLoteValidacion) -> `NO ACTION`
**The Logic:** This is "Master Data". Let's say you have a Type called "pH Test" (ID 5). You have 1,000 historical records where you tested the pH.
*   **If you CASCADE:** Deleting "pH Test" would instantly **delete all 1,000 historical records** from your database. **This is catastrophic.** You lose your history.
*   **If you SET NULL:** You keep the rows, but `IdTipo` becomes NULL. You now have a result of "7.0" but you don't know if it was pH, Temperature, or Weight. The data is useless.
*   **The Solution:** You use the default **`NO ACTION`**. This means the database will **prevent** you from deleting a Type if it is being used. Instead of deleting, you should mark the Type as Inactive (Soft Delete).

---

### The SQL Implementation

Here is the corrected table definition implementing exactly what you need:

```sql
CREATE TABLE SML_ArticuloLoteValidacionesDetalles (
    Id INT NOT NULL IDENTITY(1,1),
    
    -- Both IDs must be NOT NULL
    IdArticuloLoteValidacion INT NOT NULL,
    IdTipoArticuloLoteValidacion INT NOT NULL, 
    
    ValorEsperado VARCHAR(100) NULL,
    ValorObtenido VARCHAR(100) NULL,
    Conforme BIT NOT NULL,
    
    CONSTRAINT PK_SML_ArticuloLoteValidacionesDetalles PRIMARY KEY (Id),

    -- RELATIONSHIP 1: The Transaction Header
    -- If the Header is deleted, automatically delete these details.
    CONSTRAINT FK_SML_ALVD_ArticuloLoteValidacion 
        FOREIGN KEY (IdArticuloLoteValidacion) 
        REFERENCES SML_ArticuloLoteValidacion (Id)
        ON DELETE CASCADE, 

    -- RELATIONSHIP 2: The Master Data Type
    -- If someone tries to delete a Type that is used here, SQL Server will THROW AN ERROR.
    -- This protects your historical data.
    CONSTRAINT FK_SML_ALVD_TipoArticuloLoteValidacion 
        FOREIGN KEY (IdTipoArticuloLoteValidacion) 
        REFERENCES SML_TipoArticuloLoteValidacion (Id)
        -- ON DELETE NO ACTION -- This is the default, you don't need to type it.
);
```

### How to handle the "Type" deletion?
Since you cannot physically delete a `TipoArticuloLoteValidacion` once it has been used, you rely on the `Estado` (Status) column you created in your previous Stored Procedure.

1.  **Do not DELETE** from `SML_TipoArticuloLoteValidacion`.
2.  **UPDATE** `SML_TipoArticuloLoteValidacion` SET `Estado = 0` (Inactive).
3.  In your application dropdowns, filter to show only `WHERE Estado = 1`.

This preserves historical data while preventing the "Type" from being used in *future* validations.