# CPP-Dotenv
A small module for working with .env files in CPP.

## Installation
Simply shove the header file were it is needed and code right away, it is that simple.

```sh
curl https://github.com/cylon-dev/CPP-Dotenv/src/dotenv.hpp -o dotenv.hpp
```

## Usage
Using the module has never been easier.

### Setting up
```cpp
#include "dotenv.hpp"   // Include the header file

auto& env = dotenv::dotenv::load_dotenv();   // Intialise the class once and your ready to go
```

Now lets access your variables
```cpp
# Using the intialised class
env.getenv("Foo");
env["Foo"];

# Or use std::getenv -> Allows for environment variables to be used across files
std::getenv("Foo");
```

### Variable assigning and handling
* **Comments**: Defined by using `#` (Ignored if in quotations)
* **Strings**: type[std::string] : `VAR = "String # Here"` -> `getenv("VAR") == "String # Here"`
* **Integers**: type[int] : `VAR = 10` -> `getenv("VAR") == 10`
* **Floats**: type[double] : `VAR = 0.1` -> `getenv("VAR") == 0.1`
* **Booleans** type[bool] : `VAR = true` -> `getenv("VAR") == true`
* **NULL** type[NULL] : `VAR =` -> `getenv("VAR") == NULL`
