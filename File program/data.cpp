#include <iostream>
#include <cctype>
#include <fstream>
#include <limits>
#include <string>

#include "evakuasi.h"

using namespace std;

void simpanDataKeFile() {
	ofstream file("data_evakuasi.txt");
	if (!file.is_open()) {
		cout << "\n[P-ERROR] Gagal Membuka file untuk menyimpan data!\n";
		return;
	}

	// Format penyimpanan terstruktur berbasis teks
	file << dataKelurahanErupsi.size() << "\n";
	for (const auto& kel : dataKelurahanErupsi) {
		file << kel.id << "\n";
		file << kel.namaKelurahan << "\n";
		file << kel.titikEvakuasiAkhir << "\n";

		file << kel.daftarLandmark.size() << "\n";
		for (const auto& ta : kel.daftarLandmark) {
			file << ta.id << "\n";
			file << ta.namaLandmark << "\n";

			file << ta.daftarRute.size() << "\n";
			for (const auto& rute : ta.daftarRute) {
				file << rute.kodeRute << "\n";
				file << rute.h << "\n";
				file << rute.daftarRuas.size() << "\n";
				for (const auto& r : rute.daftarRuas) {
					file << r.namaJalan << "\n";
					file << r.jarakKM << "\n";
					file << r.arahAksi << "\n";
					file << r.patokanFisik << "\n";
					file << r.tingkatRisiko << "\n";
					file << r.catatan << "\n";
				}
			}
		}
	}
	file.close();
}

void muatDataDariFile() {
	ifstream file("data_evakuasi.txt");
	if (!file.is_open()) {
		// File belum ada (pertama kali dijalankan), abaikan
		return;
	}

	dataKelurahanErupsi.clear();
	size_t jumlahKelurahan;
	if (!(file >> jumlahKelurahan)) return;
	file.ignore(numeric_limits<streamsize>::max(), '\n');

	for (size_t i = 0; i < jumlahKelurahan; ++i) {
		Kelurahan kel;
		file >> kel.id;
		file.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(file, kel.namaKelurahan);
		getline(file, kel.titikEvakuasiAkhir);

		size_t jumlahLandmark;
		file >> jumlahLandmark;
		file.ignore(numeric_limits<streamsize>::max(), '\n');

		for (size_t j = 0; j < jumlahLandmark; ++j) {
			TitikAcuan ta;
			file >> ta.id;
			file.ignore(numeric_limits<streamsize>::max(), '\n');
			getline(file, ta.namaLandmark);

			size_t jumlahRute;
			file >> jumlahRute;
			file.ignore(numeric_limits<streamsize>::max(), '\n');

			ta.daftarRute.clear();
			for (size_t k = 0; k < jumlahRute; ++k) {
				Rute rute;
				getline(file, rute.kodeRute);
				file >> rute.h;
				size_t jumlahRuas;
				file >> jumlahRuas;
				file.ignore(numeric_limits<streamsize>::max(), '\n');

				rute.daftarRuas.clear();
				for (size_t m = 0; m < jumlahRuas; ++m) {
					RuasJalan r;
					getline(file, r.namaJalan);
					file >> r.jarakKM;
					file.ignore(numeric_limits<streamsize>::max(), '\n');
					getline(file, r.arahAksi);
					getline(file, r.patokanFisik);
					file >> r.tingkatRisiko;
					file.ignore(numeric_limits<streamsize>::max(), '\n');
					getline(file, r.catatan);
					rute.daftarRuas.push_back(r);
				}
				ta.daftarRute.push_back(rute);
			}
			kel.daftarLandmark.push_back(ta);
		}
		dataKelurahanErupsi.push_back(kel);
	}
	file.close();
}