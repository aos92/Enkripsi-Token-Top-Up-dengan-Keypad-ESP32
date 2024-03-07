#include <Arduino.h> 
#include <Keypad.h> 

#define ROW_NUM 4 
#define COLUMN_NUM 4

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pin_rows[ROW_NUM] = {19, 18, 5, 17};
byte pin_columns[COLUMN_NUM] = {16, 4, 0, 2}; 

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_columns, ROW_NUM, COLUMN_NUM ); 

unsigned char kunci[] = "1234567890"; // Kunci untuk enkripsi dan dekripsi
unsigned char destinasi[10];

unsigned char* kunci_privasi = kunci;
uint16_t panjang_kunci_privasi = 10; 
unsigned char kunci_reduksi = 0;

unsigned long top_up = 0;
unsigned int input_digit = 0;
const int max_digit = 10; 

void hitung_kunci_reduksi(unsigned char *kunci, uint16_t panjang) {
  kunci_reduksi = 0;
  
  for (uint16_t i = 0; i < panjang; i++)
    kunci_reduksi ^= (kunci[i] << (i % 8)); // Operasi XOR untuk menghasilkan kunci reduksi
}

void proses(unsigned char *token, unsigned char *destinasi, uint16_t panjang, unsigned char sisipan, unsigned char* kunci, uint16_t panjang_kunci) {
  
  unsigned char mod = sisipan ^ kunci_reduksi; // Operasi XOR antara sisipan dan kunci reduksi
 
  for (uint16_t i = 0; i < panjang; i++) {
    uint8_t superMod = mod ^ i;
    destinasi[i] = token[i] ^ superMod ^ kunci[superMod % panjang_kunci]; // Proses enkripsi atau dekripsi
  }
}

void set_kunci(unsigned char *kunci, uint16_t panjang, unsigned char* &kunci_privasi, uint16_t &panjang_kunci_privasi) {
  kunci_privasi = kunci; // Set kunci privasi baru
  panjang_kunci_privasi = panjang; // Set panjang kunci privasi baru
}

void top_up_ke_token(unsigned long be_token, unsigned char* token) {
  String str_token = String(be_token);
  while (str_token.length() < 10) {
    str_token = "0" + str_token; // Format token menjadi 10 digit
  }
  for (int i = 0; i < 10; i++) {
    token[i] = str_token.charAt(i); // Setiap karakter token disimpan dalam array token
  }
}

unsigned long token_ke_top_up(unsigned char* token) {
  unsigned long result = 0;
  int multiplier = 1;
  for (int i = 9; i >= 0; i--) {
    result += (token[i] - '0') * multiplier; // Mengubah token kembali menjadi nilai top up
    multiplier *= 10;
  }
  return result;
}

void setup() {
  Serial.begin(115200); // Inisialisasi komunikasi serial
  randomSeed(analogRead(36)); // Inisialisasi generator bilangan acak
}

void loop() {
  char key = keypad.getKey(); // Membaca input dari keypad

  if (key != NO_KEY) {
    if (key >= '0' && key <= '9' && input_digit < max_digit) {
      top_up = top_up * 10 + (key - '0'); // Membaca dan menyimpan input top up
      input_digit++;
      Serial.println(top_up); // Menampilkan nilai top up
    } else if (key == '*') {
      
      top_up /= 10; // Menghapus digit terakhir dari nilai top up
      input_digit--;
      if (top_up < 0) {
        top_up = 0; // Jika nilai top up menjadi negatif, diubah menjadi 0
        input_digit = 0;
      }
      Serial.println(top_up); // Menampilkan nilai top up
    } else if (key == '#') {
      
      Serial.println("Input selesai, Nilai top up:"); // Menampilkan pesan bahwa input selesai
      Serial.println(top_up); // Menampilkan nilai top up yang diinput

      unsigned char token[10];
      top_up_ke_token(top_up, token); // Mengkonversi nilai top up menjadi token

      unsigned long waktu = micros();
      proses(token, destinasi, 10, 0x0F, kunci_privasi, panjang_kunci_privasi); // Proses enkripsi token
      waktu = micros() - waktu;

      for (int i = 0; i < 10; i++) token[i] = 0; // Menghapus nilai token

      Serial.print("Terenkripsi:  ");
      for (int i = 0; i < 16; i++) {
        int angka_acak = random(0, 10);
        Serial.print(angka_acak); // Menampilkan karakter acak
      }
      Serial.print(" | Proses: ");
      Serial.print(waktu);
      Serial.println(" ms ");

      Serial.println("User input token");
            
      waktu = micros();
      proses(destinasi, token, 10, 0x0F, kunci_privasi, panjang_kunci_privasi); // Proses dekripsi token
      waktu = micros() - waktu; 
            
      Serial.print("Terdeskripsi: ");
      
      int startIndex = 0;
      while (token[startIndex] == '0') {
        startIndex++;
      }
      for (int i = startIndex; i < 10; i++)
        Serial.print((char)token[i]); // Menampilkan token yang sudah didekripsi

      Serial.print(" | Proses: ");
      Serial.print(waktu);
      Serial.println(" ms ");

      for (int i = 0; i < 10; i++) destinasi[i] = 0; // Menghapus nilai destinasi

      unsigned long af_token = token_ke_top_up(token); // Mengubah token menjadi nilai top up
            Serial.print("\nTop up: ");
      Serial.print(af_token);
      Serial.println(" berhasil ");
      Serial.println();
      delay(250);

      top_up = 0;
      input_digit = 0;
    }
  }
}
            