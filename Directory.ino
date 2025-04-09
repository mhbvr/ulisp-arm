/*
 * http://forum.ulisp.com/t/showing-the-files-on-an-sdcard/1266/4
 SD Card Extension
 Put it in a file Directory.ino in the same folder as the uLisp source file for your platform.
Uncomment #define extensions at the start of the main uLisp source file.
Compile and upload uLisp.

*/
  
object *fn_directory (object *args, object *env) {

#if defined(sdcardsupport)
  (void) env;
  object *result = cons(NULL, NULL);
  
  #if defined(ARDUINO_RASPBERRY_PI_PICO)
    #if defined(CPI_PICOCALC)
    if(!SD.begin(SDCARD_SS_PIN,(uint32_t) SPI_HALF_SPEED, SPI)){
      //if(!SD.begin(SDCARD_SS_PIN,tft.getSPIinstance())){
      error2(PSTR("problem init SD card"));
      return cdr(result);
    }
    #else
    SD.begin(SDCARD_SS_PIN,(uint32_t) SPI_HALF_SPEED, SPI1);
    #endif
  #else
  SD.begin(SDCARD_SS_PIN);
  #endif
  File root = SD.open("/");
  if (!root) error2(PSTR("problem reading from SD card"));
  object *ptr = result;
  while (true) {
    File entry = root.openNextFile();
    if (!entry) break;
    
    object *filename = lispstring((char*)entry.name());
    cdr(ptr) = cons(filename, NULL);
    ptr = cdr(ptr);
  };
  root.close();
  return cdr(result);
  #else
  return NULL;
  #endif
}

// Symbol names
const char stringdirectory[] PROGMEM = "directory";

// Documentation strings
const char docdirectory[] PROGMEM = "(directory)\n"
"Reads the directory at the top level of an SD card and returns\n"
"a list of the filenames.";

// Symbol lookup table
const tbl_entry_t lookup_table2[] PROGMEM = {
  { stringdirectory, fn_directory, 0200, docdirectory },
};

// Table cross-reference functions

tbl_entry_t *tables[] = {lookup_table, lookup_table2};
const unsigned int tablesizes[] = { arraysize(lookup_table), arraysize(lookup_table2) };

const tbl_entry_t *table (int n) {
  return tables[n];
}

unsigned int tablesize (int n) {
  return tablesizes[n];
}
