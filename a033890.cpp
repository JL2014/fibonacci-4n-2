// Compile : g++ -O2 a033890.cpp -o a033890 `pkg-config --cflags --libs flint`
#include <iostream>
#include <string>
#include <flint/fmpz.h>
#include <flint/arb.h>

using namespace std;

/* précision binaire: 10000 chiffres décimaux ≈ 10000 * log2(10) ≈ 33219 bits + marge de sécurité */
#define PRECISION_BITS 35000

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <n-as-decimal-string>" << endl;
        return 1;
    }

    const slong prec = PRECISION_BITS;
    fmpz_t n, n_1;
    fmpz_init(n);
    if (fmpz_set_str(n, argv[1], 10) != 0) {
        cerr << "Error: invalid value for n" << endl;
        return 2;
    }

    arb_t a, t1, t2, t3, t4, x1, x2, r, s, one, p2n;
    arb_init(a);
    arb_init(t1);
    arb_init(t2);
    arb_init(t3);
    arb_init(t4);
    arb_init(x1);
    arb_init(x2);
    arb_init(r);
    arb_init(s);
    arb_init(one);
    arb_init(p2n);

    fmpz_init(n_1);
    fmpz_sub_ui(n_1, n, 1);

    arb_set_si(a, 45);
    arb_sqrt(a, a, prec);

    /* x1 = (7 + a) */
    arb_set_si(x1, 7);
    arb_add(x1, x1, a, prec);

    /* x2 = (7 - a) */
    arb_set_si(x2, 7);
    arb_sub(x2, x2, a, prec);

    /* t1 = x1^n */
    arb_pow_fmpz(t1, x1, n, prec);

    /* t2 = x2^n */
    arb_pow_fmpz(t2, x2, n, prec);

    /* t3 = x1^(n-1) */
    arb_pow_fmpz(t3, x1, n_1, prec);

    /* t4 = x2^(n-1) */
    arb_pow_fmpz(t4, x2, n_1, prec);

    /* r = (t1 - t2) */
    arb_sub(r, t1, t2, prec);

    /* s = (t3 - t4) */
    arb_sub(s, t3, t4, prec);

    /* r += 2*s */
    arb_add(r, r, s, prec);
    arb_add(r, r, s, prec);

    /* p2n = 2^n * a */
    arb_set_si(one, 1);
    arb_mul_2exp_fmpz(p2n, one, n);
    arb_mul(p2n, p2n, a, prec);

    arb_div(r, r, p2n, prec);

    /* Affichage haute précision */
    cout << "raw = ";
    arb_printn(r, prec, 0);
    cout << endl;

    /* Arrondi contrôlé */
    arf_t mid;
    arf_init(mid);

    /* mid = milieu exact de r */
    arf_set(mid, arb_midref(r));

    fmpz_t u;
    fmpz_init(u);

    /* arrondi au plus proche */
    arf_get_fmpz(u, mid, ARF_RND_NEAR);

    /* Vérification de proximité */
    arb_t diff, tmp;
    arb_init(diff);
    arb_init(tmp);

    arb_set_fmpz(tmp, u);
    arb_sub(diff, r, tmp, prec);
    arb_abs(diff, diff);

    /* tolérance ≈ 10^{-900} */
    arb_t tol;
    arb_init(tol);
    arb_set_ui(tol, 1);
    arb_mul_2exp_si(tol, tol, -3000);

    if (!arb_lt(diff, tol)) {
        cerr << "Warning: raw value may not be close enough to an integer or precision too low" << endl;
    }

    cout << "a(";
    fmpz_print(n);
    cout << ") = ";
    fmpz_print(u);
    cout << endl;

    arf_clear(mid);
    arb_clear(diff);
    arb_clear(tmp);
    arb_clear(tol);
    fmpz_clear(u);

    flint_cleanup();
    return 0;
}
