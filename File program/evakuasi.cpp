#include "evakuasi.h"
#include <iostream>
#include <limits>
#include <cctype>

using namespace std;

// Inisialisasi Variabel Global
vector<Kelurahan> dataKelurahanErupsi;

// Fungsi validasi angka
bool isValidAngka(string& teks) {
    if (teks.empty()) return false;

    for (char c : teks) {
        if (!isdigit(c) && c != '-' && c != '.') {
            return false;
        }
    }
    return true;
}

// Fungsi validasi 
bool isValidHuruf(const string& str) {
    // 1. Jika string kosong, tidak valid
    if (str.empty()) return false;

    bool adaHurufAtauStrip = false; // Penanda apakah ada minimal satu huruf atau tanda strip
    bool semuaAngka = true;         // Penanda untuk mendeteksi apakah isinya murni angka saja

    for (char c : str) {
        // Cek apakah karakter tersebut adalah angka
        if (isdigit(c)) {
            // Jika masih berupa angka, biarkan dulu, tapi tandai
        } 
        // Cek apakah karakter adalah huruf, spasi, atau tanda strip (-)
        else if (isalpha(c) || isspace(c) || c == '-') {
            if (isalpha(c) || c == '-') adaHurufAtauStrip = true; 
            semuaAngka = false; 
        } 
        else return false;
    }

    // 2. ATURAN: Jika isinya murni angka semua (seperti "1", "123"), maka tidak valid
    if (semuaAngka) return false;

    // 3. Pastikan ada minimal huruf atau tanda strip di dalam input (mencegah hanya spasi saja)
    if (!adaHurufAtauStrip) return false;

    return true;
}

// Verifikasi password admin
bool verifikasiAdmin() {
    string password;
    cout << "\n=== VERIFIKASI ADMIN / PETUGAS RESMI ===\n";
    cout << "Masukkan Password Admin: ";
    cin >> password;
    if (password == "admin123") {
        cout << "Verifikasi Berhasil! Selamat datang, Petugas.\n";
        return true;
    } else {
        cout << "Password salah! Akses ditolak.\n";
        return false;
    }
}

// Fungsi cetak rute rapi dengan notifikasi status terkini dari Admin
void cetakDetailSatuRuteRapi(const Rute& rute, int nomorRute) {
    cout << "\n  --- Opsi Rute " << nomorRute << " [" << rute.kodeRute << "] ---\n";
    cout << "  Jarak Total Asli   : " << rute.hitungJarakAsli() << " KM\n";
    cout << "  Estimasi Waktu     : " << rute.hitungEstimasiWaktuMenit() << " Menit (Kecepatan Evakuasi 20 km/jam)\n";
    
    // Notifikasi status jalur berdasarkan update terakhir admin
    if (rute.mengandungBahayaTotal()) {
        cout << "  [NOTIFIKASI UPDATE ADMIN]: Status jalur ini TELAH DIUPDATE menjadi BERBAHAYA/PUTUS (Level 2)!\n";
    } else {
        cout << "  Status Jalur       : Aman / Bisa Dilalui\n";
    }

    cout << "  Detail Ruas Jalan:\n";
    
    int idx = 1;
    for (const auto& ruas : rute.daftarRuas) {
        cout << "    " << idx++ << ". Jalan: " << ruas.namaJalan 
             << " (" << ruas.jarakKM << " KM)\n"
             << "       Aksi/Arah     : " << ruas.arahAksi << "\n"
             << "       Patokan Fisik : " << ruas.patokanFisik << "\n"
             << "       Tingkat Risiko: ";
        if (ruas.tingkatRisiko == 0) cout << "Aman (0)\n";
        else if (ruas.tingkatRisiko == 1) cout << "Waspada / Gedung Rentan (1) [Diperbarui Petugas]\n";
        else cout << "Bahaya / Rusak Total (2) [Diperbarui Petugas]\n";
        cout << "       Catatan       : " << ruas.catatan << "\n";
    }
}

// Fungsi rekomendasi
void tampilkanSemuaRuteDanRekomendasi(const TitikAcuan& landmark, const string& namaKelurahan, const string& titikTujuan) {
    cout << "\n============================================================\n";
    cout << " EVALUASI JALUR EVAKUASI: " << landmark.namaLandmark 
         << " (Kelurahan " << namaKelurahan << ")\n";
    cout << " Tujuan Akhir Evakuasi: " << titikTujuan << "\n";
    cout << "============================================================\n";

    if (landmark.daftarRute.empty()) {
        cout << " Belum ada data rute yang dimasukkan untuk landmark ini.\n";
        cout << "============================================================\n";
        return;
    }

    for (size_t i = 0; i < landmark.daftarRute.size(); i++) {
        cetakDetailSatuRuteRapi(landmark.daftarRute[i], i+1);
    }

    cout << "\n----------------------------------------------------------\n";
    cout << " [REKOMENDASI SISTEM BERDASARKAN PEMBARUAN TERAKHIR ADMIN]:\n";
    // Mencari Rute Optimal otomatis dari daftar rute yang tidak berbahaya
    int ruteTerbaikIdx = -1;
    double minF = 1e9;
    bool semuaBahaya = true;

    for (size_t i = 0; i < landmark.daftarRute.size(); i++) {
        if (!landmark.daftarRute[i].mengandungBahayaTotal()) {
            semuaBahaya = false;
            double fVal = landmark.daftarRute[i].hitungF();
            if (fVal < minF) {
                minF = fVal;
                ruteTerbaikIdx = i;
            }
        }
    }

    if (semuaBahaya) {
        cout << " PERINGATAN DARURAT: Berdasarkan update terbaru petugas, SELURUH rute alternatif melewati jalur bahaya total! Cari titik kumpul alternatif.\n";
    } else {
        cout << " >>> REKOMENDASI UTAMA: Gunakan Rute " << (ruteTerbaikIdx + 1)
             << " [" << landmark.daftarRute[ruteTerbaikIdx].kodeRute
             << "] (Status: Terkonfirmasi Aman/Optimal dengan Cost F terkecil).\n";
    }
    cout << "============================================================\n";
}

void daftarKelurahanAdmin() {
    if (dataKelurahanErupsi.empty()) {
        cout << "\n[Info] Belum ada data kelurahan di dalam sistem.\n";
    } else {
        cout << "\n=======================================================\n";
        cout << "      DAFTAR LENGKAP KELURAHAN, RUTE, & RISIKO JALUR   \n";
        cout << "=======================================================\n";
                    
        for (size_t i = 0; i < dataKelurahanErupsi.size(); ++i) {
            const auto& kel = dataKelurahanErupsi[i];
            cout << "\n-------------------------------------------------------\n";
            cout << " KELURAHAN [" << (i + 1) << "]: " << kel.namaKelurahan << "\n";
            cout << " Titik Evakuasi Akhir  : " << kel.titikEvakuasiAkhir << "\n";
            cout << "-------------------------------------------------------\n";

            if (kel.daftarLandmark.empty()) {
                cout << " [Info] Belum ada Titik Acuan / Landmark di kelurahan ini.\n";
            } else {
                // Perulangan untuk menampilkan setiap Landmark dan detail rutenya
                for (const auto& landmark : kel.daftarLandmark) {
                    // Memanggil fungsi yang sudah ada untuk mencetak rute, ruas, dan rekomendasi secara lengkap
                    tampilkanSemuaRuteDanRekomendasi(landmark, kel.namaKelurahan, kel.titikEvakuasiAkhir);
                }
            }
        }
    }
}

// Tambah data kelurahan dinamis
void tambahDataKelurahanDinamis() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    Kelurahan k;
    k.id = dataKelurahanErupsi.size() + 1;

    cout << "\n--- TAMBAH KELURAHAN & TITIK EVAKUASI BARU ---\n";

    do {
        cout << "Masukkan Nama Kelurahan: ";
        getline(cin, k.namaKelurahan);
        // validasi input berupa huruf
        if (!isValidHuruf(k.namaKelurahan)) cout << "Input salah! Nama kelurahan hanya boleh berupa huruf atau text.\n";
    } while (!isValidHuruf(k.namaKelurahan));

    do {
        cout << "Masukkan Titik Evakuasi Akhir / Zona Aman: ";
        getline(cin, k.titikEvakuasiAkhir);
        // validasi input berupa huruf
        if (!isValidHuruf(k.titikEvakuasiAkhir)) cout << "Input salah! Nama Titik Evakuasi hanya boleh berupa huruf atau text.\n";
    } while (!isValidHuruf(k.titikEvakuasiAkhir));

    char lanjutLandmark = 'y';
    while (lanjutLandmark == 'y' || lanjutLandmark == 'Y') {
        TitikAcuan ta;
        ta.id = k.daftarLandmark.size() + 1;

        do {
            cout << "\n  -> Masukkan Nama Titik Acuan / Landmark: ";
            getline(cin, ta.namaLandmark);
            if (!isValidHuruf(ta.namaLandmark)) cout << "      Input salah! Landmark hanya boleh berupa huruf atau text.\n";
        } while (!isValidHuruf(ta.namaLandmark));

        // Input Rute Dinamis
        ta.daftarRute.clear();
        char lanjutRute = 'y';
        int ruteKe = 1;

        while (lanjutRute == 'y' || lanjutRute == 'Y') {
            Rute ruteBaru;
            cout << "     [Input Rute Alternatif Ke-" << ruteKe << "]\n";

            do {
                cout << "     Kode Rute (misal: RUTE-" << ruteKe << "): "; getline(cin, ruteBaru.kodeRute);
                if (!isValidHuruf(ruteBaru.kodeRute)) cout << "     Input salah! Kode rute hanya boleh berupa gabungan angka/huruf.\n";
            } while (!isValidHuruf(ruteBaru.kodeRute));

            string nilaiSementara;
            do {
                cout << "     Nilai Heuristik h: "; getline(cin, nilaiSementara);
                if (!isValidAngka(nilaiSementara)) cout << "     Input salah! Nilai Heuristik hanya boleh berupa angka.\n";
            } while (!isValidAngka(nilaiSementara));
            ruteBaru.h = stod(nilaiSementara);

            int jumlahRuas;
            do {
                cout << "     Jumlah ruas jalan pada Rute ini? "; getline(cin, nilaiSementara);
                if (!isValidAngka(nilaiSementara)) cout << "     Input salah! Ruas Jalan hanya boleh berupa angka.\n";
            } while (!isValidAngka(nilaiSementara));
            jumlahRuas = stoi(nilaiSementara);
            cout << endl;

            ruteBaru.daftarRuas.clear();
            for (int i = 0; i < jumlahRuas; ++i) {
                RuasJalan r;
                do {
                    cout << "       Ruas Jalan ke-" << (i + 1);
                    cout << " Nama: "; getline(cin, r.namaJalan);
                    if (!isValidHuruf(r.namaJalan)) cout << "       Input salah! Nama Jalan hanya boleh berupa huruf atau text.\n";
                } while (!isValidHuruf(r.namaJalan));

                do {
                    cout << "       Jarak (KM): "; getline(cin, nilaiSementara);
                    if (!isValidAngka(nilaiSementara)) cout << "       Input salah! Jarak harus berupa angka.\n";
                } while (!isValidAngka(nilaiSementara));   
                r.jarakKM = stod(nilaiSementara);           

                do {
                    cout << "       Arah / Aksi: "; getline(cin, r.arahAksi);
                    if (!isValidHuruf(r.arahAksi)) cout << "       Input salah! Arah/Aksi hanya boleh berupa huruf atau text.\n";
                } while (!isValidHuruf(r.arahAksi));

                do {
                    cout << "       Patokan Fisik: "; getline(cin, r.patokanFisik);
                    if (!isValidHuruf(r.patokanFisik)) cout << "       I nput salah! Patokan hanya boleh berupa huruf atau text.\n";
                } while (!isValidHuruf(r.patokanFisik));

                // PERBAIKAN DI SINI: Mengganti 'cin >> r.tingkatRisiko' dengan getline + validasi angka
                do {
                    cout << "       Tingkat Risiko (0=Aman, 1=Waspada, 2=Bahaya/Putus): "; getline(cin, nilaiSementara);
                    if (!isValidAngka(nilaiSementara) || stoi(nilaiSementara) < 0 || stoi(nilaiSementara) > 2) cout << "        Input salah! Tingkat Risiko harus berupa angka (0, 1, atau 2).\n";
                } while (!isValidAngka(nilaiSementara) || stoi(nilaiSementara) < 0 || stoi(nilaiSementara) > 2);
                r.tingkatRisiko = stoi(nilaiSementara);

                do {
                    cout << "       Catatan: "; getline(cin, r.catatan);
                    if (!isValidHuruf(r.catatan)) cout << "        Input salah! Catatan hanya boleh berupa huruf atau text.\n";
                } while (!isValidHuruf(r.catatan));
                cout << endl;
                
                ruteBaru.daftarRuas.push_back(r);
            }

            ta.daftarRute.push_back(ruteBaru);
            ruteKe++;

            cout << " Tambah rute alternatif lain untuk titik acuan ini? (y/n): ";
            cin >> lanjutRute;
            cin.ignore();
        }

        k.daftarLandmark.push_back(ta);
        cout << " Tambah Titik Acuan lain untuk kelurahan ini? (y/n): ";
        cin >> lanjutLandmark;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    dataKelurahanErupsi.push_back(k);
    simpanDataKeFile();
    cout << "\n[SUKSES] Data Kelurahan berhasil disimpan!\n";
}

// Fitur Update Status Ruas Jalan Langsung oleh Admin
void updateStatusRuasAdmin() {
    string nilaiSementara;

    if (dataKelurahanErupsi.empty()) {
        cout << "\n[Info] Belum ada data kelurahan di dalam sistem.\n";
        return;
    }

    int idxKel, idxLandmark, pilihRute, idxRuas;
    cout << "\n=== UPDATE STATUS RISIKO RUAS JALAN / JALUR ===\n";
    cout << "Pilih Kelurahan:\n";
    for (size_t i = 0; i < dataKelurahanErupsi.size(); ++i) {
        cout << (i + 1) << ". " << dataKelurahanErupsi[i].namaKelurahan << "\n";
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    do {
        cout << "Pilihan Nomor: "; getline(cin, nilaiSementara);
        if (!isValidAngka(nilaiSementara)) cout << "Input Salah!! input berupa angka\n";
    } while (!isValidAngka(nilaiSementara));
    idxKel = stoi(nilaiSementara);

    if (idxKel < 1 || idxKel > static_cast<int>(dataKelurahanErupsi.size())) {cout << "Data Tidak Valid.\n"; return;}
    
    Kelurahan& kel = dataKelurahanErupsi[idxKel - 1];
    if (kel.daftarLandmark.empty()) { cout << "Tidak ada landmark.\n"; return; }

    cout << "\nPilih Titik Acuan / Landmark:\n";
    for (size_t i = 0; i < kel.daftarLandmark.size(); ++i) {
        cout << (i + 1) << ". " << kel.daftarLandmark[i].namaLandmark << "\n";
    }

    do {
        cout << "Pilihan Nomor: "; getline(cin, nilaiSementara);
        if (!isValidAngka(nilaiSementara)) cout << "Input Salah!! input berupa angka\n";
    } while (!isValidAngka(nilaiSementara));
    idxLandmark = stoi(nilaiSementara);

    if (idxLandmark < 1 || idxLandmark > static_cast<int>(dataKelurahanErupsi.size())) {cout << "Data Tidak Valid.\n"; return;}
    TitikAcuan& landmark = kel.daftarLandmark[idxLandmark - 1];

    cout << "\nPilih Rute yang ingin diupdate:\n";
    for (size_t i = 0; i < landmark.daftarRute.size(); i++) {
        cout << (i + 1) << ". Rute" << (i + 1) << " [" << landmark.daftarRute[i].kodeRute << "]\n";
    }

    do {
        cout << "Pilih Nomor Rute: "; getline(cin, nilaiSementara);
        if (!isValidAngka(nilaiSementara)) cout << "Input Salah!! input berupa angka\n";
    } while (!isValidAngka(nilaiSementara));
    pilihRute = stoi(nilaiSementara);

    if (pilihRute < 1 || pilihRute > static_cast<int>(landmark.daftarRute.size())) {cout << "Data Tidak Valid.\n"; return;}

    Rute& targetRute = landmark.daftarRute[pilihRute - 1];

    if (targetRute.daftarRuas.empty()) { cout << "Tidak ada ruas jalan pada rute ini.\n"; return; }

    cout << "\nPilih Ruas Jalan yang ingin diubah status risikonya:\n";
    for (size_t i = 0; i < targetRute.daftarRuas.size(); ++i) {
        cout << (i + 1) << ". " << targetRute.daftarRuas[i].namaJalan 
             << " (Risiko Saat Ini: " << targetRute.daftarRuas[i].tingkatRisiko << ")\n";
    }
    
    do {
        cout << "Pilihan Nomor Ruas: "; getline(cin, nilaiSementara);
        if (!isValidAngka(nilaiSementara)) cout << "Input Salah!! input berupa angka\n";
    } while (!isValidAngka(nilaiSementara));
    idxRuas = stoi(nilaiSementara);
    
    if (idxRuas < 1 || idxRuas > static_cast<int>(targetRute.daftarRuas.size())) {cout << "Data tidak valid.\n"; return;}

    RuasJalan& ruasTarget = targetRute.daftarRuas[idxRuas - 1];

    do {
        cout << "Masukkan Tingkat Risiko Baru (0=Aman, 1=Waspada, 2=Bahaya/Rusak Total): ";
        getline(cin, nilaiSementara);
        if (!isValidAngka(nilaiSementara) || stoi(nilaiSementara) < 0 || stoi(nilaiSementara) > 2) cout << "Input salah! Update Tingkat Risiko harus berupa angka (0, 1, atau 2).\n";
    } while (!isValidAngka(nilaiSementara) || stoi(nilaiSementara) < 0 || stoi(nilaiSementara) > 2);
    ruasTarget.tingkatRisiko = stoi(nilaiSementara);
    simpanDataKeFile();

    cout << "\n[SUKSES] Status ruas jalan \"" << ruasTarget.namaJalan 
         << "\" berhasil diperbarui menjadi Risiko Level " << ruasTarget.tingkatRisiko 
         << "! Notifikasi akan langsung tampil saat warga mengecek jalur.\n";
}

// Menu Admin Tanpa Laporan Darurat
void menuAdmin() {
    if (!verifikasiAdmin()) return;

    int pilihan;
    do {
        cout << "\n============================================\n";
        cout << "       PANEL KONTROL ADMIN / PETUGAS        \n";
        cout << "   MITIGASI BENCANA GUNUNG GAMALAMA TERNATE \n";
        cout << "============================================\n";
        cout << "1. Tambah Data Kelurahan & Titik Acuan Baru\n";
        cout << "2. Update Langsung Status Risiko Ruas Jalan\n";
        cout << "3. Lihat Daftar Kelurahan Saat Ini\n";
        cout << "4. Kembali ke Menu Utama\n";
        cout << "Pilihan Anda: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: tambahDataKelurahanDinamis(); break;
            case 2: updateStatusRuasAdmin(); break;
            case 3: daftarKelurahanAdmin(); break;
            case 4: cout << "Keluar dari Panel Admin.\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while (pilihan != 4);
}