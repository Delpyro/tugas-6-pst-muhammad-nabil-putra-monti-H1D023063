# Tugas 6 Pemrograman Sistem Tertanam - Smart Scoreboard RTOS

**Oleh:** Muhammad Nabil Putra Monti (H1D023063)  
**Program Studi:** Informatika, Universitas Jenderal Soedirman  

## Deskripsi Proyek
Proyek ini adalah simulasi **Sistem Papan Skor dan Waktu Pertandingan (Smart Scoreboard)** menggunakan mikrokontroler Arduino Uno dan sistem operasi waktu nyata (RTOS) berbasis **FreeRTOS**. 

Sistem ini dirancang untuk menyelesaikan masalah *lag* atau *delay* pada sistem tertanam tradisional. Dengan menggunakan arsitektur multitasking, penghitungan waktu mundur (*countdown timer*) dan pembaruan skor dari input tombol dapat berjalan secara sinkron, paralel, dan *non-blocking*.

## Komponen yang Digunakan
*   1x Arduino Uno R3
*   1x LCD 16x2 (dengan Modul I2C)
*   3x Push Button (Gol A, Gol B, Pause/Play)
*   Kabel Jumper secukupnya

## Simulasi Wokwi
Kode ini dapat dijalankan langsung menggunakan simulator Wokwi dengan pustaka eksternal `FreeRTOS` dan `LiquidCrystal I2C`.



https://github.com/user-attachments/assets/2c4bb648-f36b-44c4-ba17-bb379f9a45b1

