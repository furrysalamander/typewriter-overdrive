//const char* symbol_list = ",.ersaioctmlhpnbfgukvdyzqxjw/%1324567809$½¶¼#>±<¢§³@!]°[²R'Q_OJG?D\"C-FE+BV&YATLSPZ*X+K)H(UIN;W:M";

const char* symbol_list = ",.ersaioctmlhpnbfgukvdyzqxjw/%1324567809$~~~#>~<~~~@!]~[~R'Q_OJG?D\"C-FE+BV&YATLSPZ*X+K)H(UIN;W:M";
uint8_t symbol_table[256];

void build_symbol_table() {
  //Serial.print("Size of symbol_list: ");
  //Serial.println(sizeof(symbol_list));
  //Serial.println(symbol_list);
  for (int i = 0; i < 256; i++) {
    symbol_table[i] = 0xFF;
  }
  for (int i = 0; symbol_list[i] != 0; i++) {
    //Serial.println((uint8_t)symbol_list[i]);
    symbol_table[(uint8_t)symbol_list[i]] = i;
  }
}
