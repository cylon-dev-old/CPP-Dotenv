# CPP-Dotenv
A small module for working with .env files in CPP.

## Installation
Simply shove the header file were it is needed and code right away, it is that simple.

```sh
curl https://raw.githubusercontent.com/cylon-dev/CPP-Dotenv/main/src/dotenv.hpp -o dotenv.hpp
```

## Usage
Using the module has never been easier.

### Setting up
```cpp
#include "dotenv.hpp"   // Include the header file

auto env = dotenv::dotenv::load_dotenv();   // Intialise the class once and your ready to go
```

Now lets access your variables
```cpp
// Using the intialised class
env.getenv("Foo");
env["Foo"];

// Or use std::getenv || READ NEXT SECTION!!!
std::getenv("Foo");

// To maintain type conversions, the getenv method in dotenv is static :D
dotenv::dotenv::get_env("Foo");
```

### Variable assigning and handling
*NOTE: Results returned will be of the envItem class, for parsed items use the .parse method!*

*NOTE: Since return type is a varient, using `std::get<TYPE>(getenv("VAR").parse())` may help*

* **Comments**: Defined by using `#` (Ignored if in quotations)
* **Strings**: type[std::string] : `VAR = "String # Here"` -> `std::get<std::string>(getenv("VAR").parse()) == "String # Here"`
* **Integers**: type[int] : `VAR = 10` -> `std::get<int>(getenv("VAR").parse()) == 10`
* **Floats**: type[double] : `VAR = 0.1` -> `std::get<double>(getenv("VAR").parse()) == 0.1`
* **Booleans** type[bool] : `VAR = true` -> `getenv(std::get<bool>(getenv("VAR").parse())).item == true` | [true or false]
* **NULL** type[NULL] : `VAR =` -> `std::get<long int>(getenv("VAR").parse()) == NULL`

> ⚠️ Variable types will only be handled if your using `dotenv::dotenv::getenv`
You will only get handled types by using the classes getenv method.

The regular `std::getenv` will return all vars with the type `char*`

* Strings will be fetched with quotations wrapped around
* int, float and bool will all remain the same `char* var = "10" "0.1" "true"`
* NULL will be equal to `char* var = "01a"`

### Adding new variables
```cpp
// Using the class :: Works statically as well :O
env::setenv(NAME, VAR);

// Standard function will also work, You just wont get any verification before its added
setenv(NAME, VAR);
```

### Deleting variables
```cpp
// Using class :: Works statically
env::delvar(VAR);

// Standard
setenv(VAR, NULL, 1);
```

### Editing existing variables
```cpp
// Using class
env::setenv(VAR, VALUE, true);

// Standard
setenv(VAR, VALUE, FLAG);
```

### Clearing environment
```cpp
// Using class
env::clearenv();

// Standard
clearenv();
```

## Error Reporting
This little file can also return errors and warnings whilst dealing with setting your environment.
```
EINVAL - Raised name is NULL (empty) or is pointing to a string with `=` in it
ENOMEM - Raised when there is insufficient memory available
```

For any other errors that may lurk, they will also be raised with only the context of the error.

## Signatures

### envItem [dotenv::envItem]
```cpp
// Attributes
envItem.item        // Raw item before any parsing
envItem.can_parse   // Whether the item can be parsed
```

**dotenv::envItem::create**
```cpp
static envItem create(
    std::string item, 
    bool can_parse
);
```

**dotenv::envItem::parse**
```cpp
std::variant<int, double, bool, std::string, long int> parse();
```

### dotenv [dotenv::dotenv]

**dotenv::dotenv::load_dotenv**
```cpp
static dotenv load_dotenv(
    std::string dotenv_path = "./.env",           // path to .env file
    std::map<std::string, std::string> vars = {}, // additional variables that are not found in .env
                                                  // - Will overwrite any variables from the .env file
    bool overwrite = false,                       // whether to overwrite exisiting enviroment variables
    bool ignore_whitespace = true,                // whether to ignore lines that just contain whitespace
    bool warn_on_overwrite = true,                // whether to warn before overwriting variable
    bool warn_on_error = true,                    // whether to warn when an error occurs through parsing
    bool throw_err = false,                       // whether to throw errors which can be critical - .env file can't opened
    bool warn_on_set_err = true                   // whether to disable error tracking
);
```

**dotenv::dotenv::parse_env**
```cpp
void parse_env();
```

**dotenv::dotenv::load_variables**
```cpp
void load_variables();
```

**dotenv::dotenv::setenv**
```cpp
void setenv(
    std::string key, 
    std::string value, 
    bool __overwrite = false
);
```

**dotenv::dotenv::getenv**
```cpp
static envItem getenv(
    std::string key,               // Item to fetch
    bool can_be_parsed = true     // Whether fetched item can be parsed
);
```

**dotenv::dotenv::delvar**
```cpp
static bool delvar(std::string key);
```

**dotenv::dotenv::clearenv**
```cpp
static bool clearenv();
```
