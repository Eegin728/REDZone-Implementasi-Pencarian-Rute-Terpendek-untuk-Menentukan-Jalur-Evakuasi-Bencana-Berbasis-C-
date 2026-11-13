#ifndef EVAKUASI_H
#define EVAKUASI_H

#include <string>
#include <vector>

using namespace std;

// Struktur data Ruas Jalan / Segmen
struct RuasJalan {
    string namaJalan;
    double jarakKM;
    string arahAksi;     
    string patokanFisik; 
    int tingkatRisiko;   // 0 = Aman, 1 = Gedung Rentan, 2 = Bahaya/Rusak Total
    string catatan;
};

// Struktur data Rute Alternatif dengan Algoritma A*
struct Rute {
    string kodeRute;
    vector<RuasJalan> daftarRuas;
    double h; // Heuristic (estimasi jarak sisa)

    double hitungG() const {
        double totalG = 0;
        for (const auto& ruas : daftarRuas) {
            totalG += ruas.jarakKM;
            if (ruas.tingkatRisiko == 1) {
                totalG += 2.0;       
            } else if (ruas.tingkatRisiko == 2) {
                totalG += 50.0;      
            }
        }
        return totalG;
    }

    double hitungJarakAsli() const {
        double total = 0;
        for (const auto& ruas : daftarRuas) total += ruas.jarakKM;
        return total;
    }

    double hitungF() const {
        return hitungG() + h; 
    }

    int hitungEstimasiWaktuMenit() const {
        return static_cast<int>((hitungJarakAsli() / 20.0) * 60.0);
    }
    
    bool mengandungBahayaTotal() const {
        for (const auto& ruas : daftarRuas) {
            if (ruas.tingkatRisiko == 2) return true;
        }
        return false;
    }
};

struct TitikAcuan {
    int id;
    string namaLandmark;
    vector<Rute> daftarRute;
};

struct Kelurahan {
    int id;
    string namaKelurahan;
    string titikEvakuasiAkhir; 
    vector<TitikAcuan> daftarLandmark;
};

// Variabel Global
extern vector<Kelurahan> dataKelurahanErupsi;

// Prototipe fungsi penyimpanan data
void simpanDataKeFile();
void muatDataDariFile();

// Prototipe Fungsi
bool isValidAngka(string& teks);
bool verifikasiAdmin();
void cetakDetailSatuRuteRapi(const Rute& rute, int nomorRute);
void tampilkanSemuaRuteDanRekomendasi(const TitikAcuan& landmark, const string& namaKelurahan, const string& titikTujuan);
void tambahDataKelurahanDinamis();
void updateStatusRuasAdmin();
void menuAdmin();
void menuUser();

#endif