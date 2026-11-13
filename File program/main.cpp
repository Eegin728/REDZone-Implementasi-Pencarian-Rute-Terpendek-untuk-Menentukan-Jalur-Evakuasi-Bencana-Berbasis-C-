#include <iostream>
#include <limits>
#include "evakuasi.h"

using namespace std;

string nilaiSementara;

// Menu Publik / User untuk Melihat Rute dan Notifikasi Update Terbaru dari Admin
void menuMasyarakat() {
    if (dataKelurahanErupsi.empty()) {
        cout << "\n[Perhatian] Data kelurahan dan rute belum diinput oleh Admin/Petugas Resmi.\n";
        return;
    }

    int idxKel;
    cout << "\n=== PILIH KELURAHAN ANDA SAAT INI ===\n";
    for (size_t i = 0; i < dataKelurahanErupsi.size(); ++i) {
        cout << (i + 1) << ". Kelurahan " << dataKelurahanErupsi[i].namaKelurahan << "\n";
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    do {
        cout << "Pilihan (Nomor): ";
        getline(cin, nilaiSementara);
        if (!isValidAngka(nilaiSementara)) cout << "Input Salah!! Input harus berupa Angka.\n";
    } while (!isValidAngka(nilaiSementara));
    idxKel = stoi(nilaiSementara);

    if (idxKel < 1 || idxKel > static_cast<int>(dataKelurahanErupsi.size())) {
        cout << "Pilihan kelurahan tidak valid!\n";
        return;
    }

    const Kelurahan& kelPilihan = dataKelurahanErupsi[idxKel - 1];

    if (kelPilihan.daftarLandmark.empty()) {
        cout << "\n[Info] Belum ada titik acuan/landmark di kelurahan ini.\n";
        return;
    }

    int idxLandmark;
    cout << "\n=== PILIH POSISI / TITIK ACUAN TERDEKAT ANDA ===\n";
    for (size_t i = 0; i < kelPilihan.daftarLandmark.size(); ++i) {
        cout << (i + 1) << ". " << kelPilihan.daftarLandmark[i].namaLandmark << "\n";
    }
    
    do {
        cout << "Pilihan (Nomor): ";
        getline(cin, nilaiSementara);
        if (!isValidAngka(nilaiSementara)) cout << "Input Salah!! Input harus berupa Angka.\n";
    } while (!isValidAngka(nilaiSementara));
    idxLandmark = stoi(nilaiSementara);

    if (idxLandmark < 1 || idxLandmark > static_cast<int>(kelPilihan.daftarLandmark.size())) {
        cout << "Pilihan landmark tidak valid!\n";
        return;
    }

    const TitikAcuan& landmarkPilihan = kelPilihan.daftarLandmark[idxLandmark - 1];

    // Menampilkan evaluasi rute lengkap dengan notifikasi status update dari admin
    tampilkanSemuaRuteDanRekomendasi(landmarkPilihan, kelPilihan.namaKelurahan, kelPilihan.titikEvakuasiAkhir);
}

int main() {
    muatDataDariFile();
    int menuUtama;
    do {
        cout << "\n=======================================================\n";
        cout << "  SISTEM PENDUKUNG KEPUTUSAN JALUR EVAKUASI ERUPSI     \n";
        cout << "               GUNUNG GAMALAMA TERNATE                 \n";
        cout << "=======================================================\n";
        cout << "1. Masuk sebagai Pengguna / Warga (Cari Jalur Evakuasi)\n";
        cout << "2. Masuk sebagai Admin / Petugas Resmi (Panel Kontrol)\n";
        cout << "3. Keluar Program\n";
        cout << "Pilihan Menu: ";
        cin >> menuUtama;

        switch (menuUtama) {
            case 1:
                menuMasyarakat();
                break;
            case 2:
                menuAdmin();
                break;
            case 3:
                cout << "\nTerima kasih. Tetap waspada terhadap aktivitas Gunung Gamalama!\n";
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }
    } while (menuUtama != 3);

    return 0;
}