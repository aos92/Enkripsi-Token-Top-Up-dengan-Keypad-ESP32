# Enkripsi Token Top Up dengan Keypad ESP32

Repositori ini berisi skrip untuk ESP32 yang memungkinkan pengguna untuk memasukkan nilai top up melalui keypad dan mengenkripsi token menggunakan algoritma XOR dengan sebuah kunci tertentu. Skrip ini juga menyediakan fungsi untuk mendekripsi token dan mengonversinya kembali ke nilai top up.

## Persyaratan
- Papan pengembangan ESP32
- Modul keypad
- Arduino IDE

## Penggunaan
1. Hubungkan modul keypad ke papan pengembangan ESP32.
2. Unggah skrip ke ESP32 menggunakan Arduino IDE.
3. Gunakan keypad untuk memasukkan nilai top up yang diinginkan.
4. Tekan tombol '#' untuk menyelesaikan memasukkan nilai dan menghasilkan token.
5. Token yang terenkripsi akan ditampilkan, diikuti oleh token yang didekripsi dan nilai top up yang sesuai.

## Konfigurasi
- Modifikasi array `keys` untuk menyesuaikan tata letak keypad.
- Sesuaikan array `pin_rows` dan `pin_columns` dengan koneksi pin dari modul keypad Anda.
- Ubah array `kunci` untuk menetapkan kunci enkripsi yang berbeda.

## Lisensi
Proyek ini dilisensikan di bawah Lisensi MIT - lihat file [LICENSE](LICENSE) untuk detailnya.
