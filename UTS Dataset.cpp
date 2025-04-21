#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct Paper {
    string judul;
    string tahun;
    string penulis;
};

vector<Paper> data;

string toLower(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Fungsi membaca file CSV (disesuaikan dengan dataset kamu)
void bacaCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Gagal membuka file: " << filename << "\n";
        return;
    }

    string line;
    int lineCount = 0;

    while (getline(file, line)) {
        if (lineCount++ < 2) continue; // Lewati 2 baris header

        stringstream ss(line);
        string temp, judul, tahun, penulis;

        // Lewati 5 kolom awal
        for (int i = 0; i < 5; ++i) getline(ss, temp, ',');

        getline(ss, judul, ',');
        getline(ss, tahun, ',');
        getline(ss, penulis, ',');

        // Bersihkan karakter newline / carriage return
        if (!penulis.empty() && (penulis.back() == '\r' || penulis.back() == '\n'))
            penulis.pop_back();

        // Simpan hanya jika judul tidak kosong
        if (!judul.empty()) {
            data.push_back({judul, tahun, penulis});
        }
    }

    file.close();
}

// Linear Search
void linearSearch(string keyword, string field) {
    keyword = toLower(keyword);
    bool found = false;
    cout << "Hasil Linear Search:\n";
    for (int i = 0; i < data.size(); i++) {
        string value = (field == "judul") ? data[i].judul :
                       (field == "tahun") ? data[i].tahun :
                                            data[i].penulis;
        if (toLower(value).find(keyword) != string::npos) {
            cout << i + 1 << ". " << data[i].judul << " | " << data[i].tahun << " | " << data[i].penulis << "\n";
            found = true;
        }
    }
    if (!found) cout << "Data tidak ditemukan.\n";
}

// Binary Search
void binarySearch(string keyword, string field) {
    keyword = toLower(keyword);
    sort(data.begin(), data.end(), [field](Paper a, Paper b) {
        return toLower((field == "judul") ? a.judul :
                       (field == "tahun") ? a.tahun :
                                            a.penulis)
             <
               toLower((field == "judul") ? b.judul :
                       (field == "tahun") ? b.tahun :
                                            b.penulis);
    });

    int left = 0, right = data.size() - 1;
    bool found = false;
    cout << "Hasil Binary Search:\n";
    while (left <= right) {
        int mid = (left + right) / 2;
        string value = (field == "judul") ? data[mid].judul :
                       (field == "tahun") ? data[mid].tahun :
                                            data[mid].penulis;
        string valLower = toLower(value);

        if (valLower == keyword) {
            // Tampilkan semua yang cocok (duplikat)
            int midKiri = mid;
            while (midKiri >= 0 && toLower((field == "judul") ? data[midKiri].judul :
                                           (field == "tahun") ? data[midKiri].tahun :
                                                                data[midKiri].penulis) == keyword) {
                midKiri--;
            }

            int midKanan = mid;
            while (midKanan < data.size() && toLower((field == "judul") ? data[midKanan].judul :
                                                      (field == "tahun") ? data[midKanan].tahun :
                                                                           data[midKanan].penulis) == keyword) {
                cout << data[midKanan].judul << " | " << data[midKanan].tahun << " | " << data[midKanan].penulis << "\n";
                midKanan++;
            }

            found = true;
            break;
        } else if (valLower < keyword) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    if (!found) cout << "Data tidak ditemukan.\n";
}

int main() {
    string filename = "Struktur_Data_Dataset_Kelas_A_B_C - Sheet1.csv";
    bacaCSV(filename);

    if (data.empty()) {
        cout << "File kosong atau tidak ditemukan!\n";
        return 1;
    }

    int metode, kategori;
    string keyword, field;

    do {
        cout << "\n=== MENU PENCARIAN ===\n";
        cout << "1. Linear Search\n";
        cout << "2. Binary Search\n";
        cout << "0. Keluar\n";
        cout << "Pilih metode pencarian: ";
        cin >> metode;
        cin.ignore();

        if (metode == 0) break;
        if (metode != 1 && metode != 2) {
            cout << "Pilihan tidak valid.\n";
            continue;
        }

        cout << "\nPilih kategori:\n";
        cout << "1. Judul Paper\n";
        cout << "2. Tahun Terbit\n";
        cout << "3. Nama Penulis\n";
        cout << "Pilih: ";
        cin >> kategori;
        cin.ignore();

        if (kategori == 1) field = "judul";
        else if (kategori == 2) field = "tahun";
        else if (kategori == 3) field = "penulis";
        else {
            cout << "Kategori tidak valid.\n";
            continue;
        }

        cout << "Masukkan keyword: ";
        getline(cin, keyword);

        if (metode == 1)
            linearSearch(keyword, field);
        else
            binarySearch(keyword, field);

    } while (true);

    cout << "Terima kasih!\n";
    return 0;
}

