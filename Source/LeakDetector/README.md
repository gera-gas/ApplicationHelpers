## LeakDetector
Helpers for detection of memory leaks.

## Usage
Details example of using in code you can see [here](../../Demo/demo_leaked.cpp).   
For fast remove macroses `ADD_TO_SUSPICIOUS_OBJECTS_LIST` from code you can use `Ruby` and `Bash` scripts.
* Set start directory in field `:start_directory` (file `clean_sources.rb`).
* Set file extensions for searching in field `:file_extension` (file `clean_sources.rb`).
* OPTION: Set exclude of subfolder names in field `:exclude_folders` (file `clean_sources.rb`).
* Run `clean_sources.sh` bash file.   

    $ ./clean_sources.sh

After that your code will be automatically cleared from `ADD_TO_SUSPICIOUS_OBJECTS_LIST`.
