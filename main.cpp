#include <bits/stdc++.h>
using namespace std;

/*
  Inisialisasi I/O cepat.
*/
void Init() {
  srand(time(0));
  ios::sync_with_stdio(false);
  cin.tie(NULL);
}

/*
  Arah proyeksi yang digunakan untuk membandingkan titik.
*/
int64_t cmpx = 1, cmpy = 0;

/*
  Struktur Point merepresentasikan sebuah vektor (x, y).
*/
struct Point {
  int64_t x, y;

  // Negasi vektor
  Point operator-() const { return {-x, -y}; }

  // Penjumlahan vektor
  Point& operator+=(const Point& p) {
    x += p.x; 
    y += p.y;
    return *this;
  }

  // Pengurangan vektor
  Point operator-(const Point& p) const { return {x-p.x, y-p.y}; }

  // Penjumlahan vektor
  Point operator+(const Point& p) const { return {x+p.x, y+p.y}; }

  /*
    Perbandingan berdasarkan proyeksi ke arah (cmpx, cmpy).
    Digunakan untuk mencari titik ekstrem.
  */
  bool operator<(const Point& p) const {
    return x*cmpx + y*cmpy < p.x*cmpx + p.y*cmpy;
  }

  // Pengecekan kesamaan titik
  bool operator==(const Point& p) const {
    return x == p.x && y == p.y;
  }

  // Menghasilkan vektor ortogonal
  Point ortho() const { return {-y, x}; }

  // Menghitung kuadrat panjang vektor
  int64_t lensqr() const {
    return x*x + y*y;
  }
};

int main() {
  Init();

  int N, M;

  // Membaca sejumlah test case
  while (cin >> N) {

    // ch[i] menyimpan daftar anak node i
    vector<vector<int>> ch(N+1);

    // p[i] menyimpan nilai vektor pada node leaf
    vector<Point> p(N+1);

    // Membaca struktur pohon
    for (int i = 1; i <= N; i++) {
      cin >> M;
      if (M == 0) {
        // Node leaf
        cin >> p[i].x >> p[i].y;
      } else {
        // Node internal
        ch[i].resize(M);
        for (auto& x : ch[i]) cin >> x;
      }
    }

    // Menyimpan nilai maksimum x^2 + y^2
    int64_t ret = 0;

    /*
      Fungsi tryAngle:
      Menghitung titik minimum dan maksimum
      untuk suatu arah proyeksi tertentu.
    */
    auto tryAngle = [&](Point dir) -> pair<Point, Point> {

      // Set arah proyeksi global
      cmpx = dir.x;
      cmpy = dir.y;

      /*
        Fungsi doit(x):
        Mengembalikan pasangan (min, max) vektor
        yang mungkin dihasilkan pada subtree x.
      */
      function<pair<Point,Point>(int)> doit =
      [&](int x) -> pair<Point,Point> {

        // Node leaf hanya memiliki satu kemungkinan
        if (ch[x].empty())
          return {p[x], p[x]};

        // Inisialisasi dari anak pertama
        auto [mntot, mxtot] = doit(ch[x][0]);

        // Variabel bantu untuk menggabungkan hasil anak
        Point mndiff = mxtot + mntot;
        Point mxdiff = mndiff;

        // Menggabungkan seluruh anak
        for (int i = 1; i < ch[x].size(); i++) {
          auto [mn, mx] = doit(ch[x][i]);
          mntot += mn;
          mxtot += mx;
          mndiff = min(mndiff, mx + mn);
          mxdiff = max(mxdiff, mx + mn);
        }

        // Menghasilkan pasangan ekstrem
        return {-mxtot + mndiff, -mntot + mxdiff};
      };

      // Hitung ekstrem di root
      auto [mn, mx] = doit(1);

      // Perbarui nilai maksimum global
      ret = max(ret, mx.lensqr());
      ret = max(ret, mn.lensqr());

      return {mn, mx};
    };

    /*
      Fungsi traceHull:
      Menelusuri seluruh titik ekstrem (convex hull)
      menggunakan pembagian rekursif.
    */
    function<void(Point,Point)> traceHull =
    [&](Point a, Point b) {
      if (a == b) return;

      // Cari titik ekstrem pada arah ortogonal
      auto [_, c] = tryAngle((b - a).ortho());

      // Jika ditemukan titik di luar segmen, lanjutkan rekursi
      if (a < c) {
        traceHull(a, c);
        traceHull(c, b);
      }
    };

    // Mencari titik ekstrem awal pada arah horizontal
    auto [left, right] = tryAngle({1, 0});

    // Menelusuri seluruh convex hull
    traceHull(left, right);
    traceHull(right, left);

    // Mencetak hasil akhir
    printf("%lld\n", ret);
  }
}
