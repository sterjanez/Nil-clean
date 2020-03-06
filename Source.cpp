// Program: Nil-clean Matrices over F_2
// Author: Janez Ster
// Date: March 2020
// Description: Given a nxn matrix A over F_2, the program finds an idempotent
// matrix E over F_2 such that (A-E)^3 = 0.

#include <iostream>
#include <string>
#include <time.h>

constexpr auto MAX_DIM = 11; // maximal matrix size

// Class describing m�n matrix over F_2.
class Matrix {
public:

	// Matrix height.
	int Height;
	
	// Matrix width.
	int Width;

	// Matrix entries (true = 1, false = 0).
	bool Entry[MAX_DIM][MAX_DIM];

	// Create matrix of undefined dimensions and entries.
	Matrix() {}

	// Create m�n matrix with undefined entries.
	Matrix(int m, int n) {
		Height = m;
		Width = n;
	}

	// Create n�n matrix with undefined entries.
	Matrix(int n) : Matrix(n, n) {}

	// Assign m�n matrix to diagonal matrix with diagonal entries equal to value.
	Matrix& operator =(const int& value) {
		for (int i = 0; i < Height; i++)
			for (int j = 0; j < Width; j++)
				Entry[i][j] = (i == j && value == 1);
		return *this;
	}

	// Create matrix from a string.
	// Example: "10,01" presents 2�2 identity.
	Matrix(std::string s) {
		auto pos = s.find(",");
		if (pos == std::string::npos) {
			Height = 1;
			Width = s.length();
		}
		else {
			Height = (s.length() + 1) / (pos + 1);
			Width = pos;
		}
		for (int i = 0; i < Height; i++)
			for (int j = 0; j < Width; j++)
				Entry[i][j] = (s[i * (Width + 1) + j] == '1');
	}

	// Convert matrix to string.
	// Example: 2�2 identity is converted to "10,01".
	std::string ToString() const {
		std::string s = "";
		for (int i = 0; i < Height; i++) {
			for (int j = 0; j < Width; j++)
				s += (Entry[i][j] ? "1" : "0");
			if (i != Height - 1) s += ",";
		}
		return s;
	}

	// Matrix sum.
	Matrix operator +(const Matrix& A) const {
		Matrix B = *this;
		for (int i = 0; i < Height; i++)
			for (int j = 0; j < Width; j++)
				if (A.Entry[i][j]) B.Entry[i][j] = !B.Entry[i][j];
		return B;
	}

	// Sum of a matrix and a diagonal matrix with given value.
	Matrix operator +(int value) const {
		Matrix B(Height, Width);
		B = value;
		return (*this) + B;
	}

	// Matrix product.
	Matrix operator *(const Matrix& A) const {
		Matrix B(Height, A.Width);
		for (int i = 0; i < Height; i++)
			for (int j = 0; j < A.Width; j++) {
				bool sum = false;
				for (int k = 0; k < Width; k++)
					if (Entry[i][k] && A.Entry[k][j]) sum = !sum;
				B.Entry[i][j] = sum;
			}
		return B;
	}

	// Check if equally sized matrices are equal.
	bool operator ==(const Matrix& A) const {
		for (int i = 0; i < Height; i++)
			for (int j = 0; j < Width; j++)
				if (Entry[i][j] != A.Entry[i][j]) return false;
		return true;
	}

	// Check if a matrix is equal to diagonal matrix with diagonal entries given by value.
	bool operator ==(int value) const {
		for (int i = 0; i < Height; i++)
			for (int j = 0; j < Width; j++)
				if (Entry[i][j] != (i == j && value == 1)) return false;
		return true;
	}

	// Matrix transpose.
	Matrix Trans() const {
		Matrix A(Width, Height);
		for (int i = 0; i < Height; i++)
			for (int j = 0; j < Width; j++)
				A.Entry[j][i] = Entry[i][j];
		return A;
	}

	// Trace of a m�n matrix.
	bool Trace() const {
		bool value = false;
		for (int i = 0; i < Height && i < Width; i++)
			if (Entry[i][i]) value = !value;
		return value;
	}

	// Submatrix of size m�n with the upper left corner on position (i0, j0).
	Matrix SubMatrix(int m, int n, int i0, int j0) const {
		Matrix A(m, n);
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++)
				A.Entry[i][j] = Entry[i + i0][j + j0];
		return A;
	}

	// Expand matrix to get a block-diagonal matrix with the lower right block A.
	Matrix Diag(const Matrix& A) const {
		Matrix B(Height + A.Height, Width + A.Width);
		for (int i = 0; i < B.Height; i++)
			for (int j = 0; j < B.Width; j++)
				if (i < Height && j < Width)
					B.Entry[i][j] = Entry[i][j];
				else if (i >= Height && j >= Width)
					B.Entry[i][j] = A.Entry[i - Height][j - Width];
				else
					B.Entry[i][j] = false;
		return B;
	}

private:

	// Swap rows.
	void RowSwap(int i, int j) {
		for (int k = 0; k < Width; k++) {
			bool value = Entry[i][k];
			Entry[i][k] = Entry[j][k];
			Entry[j][k] = value;
		}
	}

	// Swap columns.
	void ColSwap(int i, int j) {
		for (int k = 0; k < Height; k++) {
			bool value = Entry[k][i];
			Entry[k][i] = Entry[k][j];
			Entry[k][j] = value;
		}
	}

	// Add i-th row to j-th row.
	void AddRow(int i, int j) {
		for (int k = 0; k < Width; k++)
			if (Entry[i][k]) Entry[j][k] = !Entry[j][k];
	}

	// Add i-th row of A to the j-th row of the owner.
	void AddRow(int i, int j, const Matrix& A) {
		for (int k = 0; k < Width; k++)
			if (A.Entry[i][k]) Entry[j][k] = !Entry[j][k];
	}

	// Add i-th column to j-th column.
	void AddCol(int i, int j) {
		for (int k = 0; k < Height; k++)
			if (Entry[k][i]) Entry[k][j] = !Entry[k][j];
	}

	// Add i-th column of A to the j-th column of the owner.
	void AddCol(int i, int j, const Matrix& A) {
		for (int k = 0; k < Height; k++)
			if (A.Entry[k][i]) Entry[k][j] = !Entry[k][j];
	}

	// Find a solution of the matrix equation AX = B, where A is the method owner.
	// Return true if a solution exists and false otherwise.
	bool Solve(Matrix B, Matrix& X) const {
		Matrix A = *this;
		int rank = 0; // will grow
		for (int j0 = 0; j0 < A.Width; j0++) {
			for (int i0 = rank; i0 < A.Height; i0++)
				if (A.Entry[i0][j0]) { // found pivot
					A.RowSwap(i0, rank);
					B.RowSwap(i0, rank);
					for (int i = 0; i < A.Height; i++)
						if (A.Entry[i][j0] && i != rank) {
							A.AddRow(rank, i);
							B.AddRow(rank, i);
						}
					rank++;
					break;
				}
		}
		for (int i = rank; i < A.Height; i++)
			for (int j = 0; j < B.Width; j++)
				if (B.Entry[i][j]) return false;
		X.Height = A.Width;
		X.Width = B.Width;
		X = 0;
		for (int i = 0; i < rank; i++)
			for (int j = 0; j < A.Width; j++)
				if (A.Entry[i][j]) {
					X.AddRow(i, j, B);
					break;
				}
		return true;
	}

public:

	// Inverse of a square invertible matrix.
	Matrix Inv() const {
		Matrix Id(Height);
		Id = 1;
		Matrix X;
		Solve(Id, X);
		return X;
	}

	// Given matrix A, find invertible square matrices U, V such that
	// UAV = [I0;00], and return rank of A.
	int Diagonalize(Matrix& U, Matrix& V) const {
		Matrix D = *this;
		U = Matrix(Height);
		V = Matrix(Width);
		U = 1;
		V = 1; // we shall maintain equality UAV = D
		int rank = 0;
		bool found_pivot;
		do {
			found_pivot = false;
			for (int i0 = rank; i0 < Height && !found_pivot; i0++)
				for (int j0 = rank; j0 < Width; j0++)
					if (D.Entry[i0][j0]) {
						D.RowSwap(i0, rank);
						U.RowSwap(i0, rank);
						D.ColSwap(j0, rank);
						V.ColSwap(j0, rank); // move pivot to position (rank, rank)
						for (int i = rank + 1; i < Height; i++)
							if (D.Entry[i][rank]) {
								D.AddRow(rank, i);
								U.AddRow(rank, i);
							}
						for (int j = rank + 1; j < Width; j++)
							if (D.Entry[rank][j]) {
								D.AddCol(rank, j);
								V.AddCol(rank, j);
							}
						rank++;
						found_pivot = true;
						break;
					}
		} while (found_pivot);
		return rank;
	}

	// Check if the image is contained in the image of A.
	bool HasImageContainedIn(Matrix A) const {
		Matrix B = *this;
		int rank = 0; // will grow
		for (int j0 = 0; j0 < A.Width; j0++) {
			for (int i0 = rank; i0 < A.Height; i0++)
				if (A.Entry[i0][j0]) { // found pivot
					A.RowSwap(i0, rank);
					B.RowSwap(i0, rank);
					for (int i = rank + 1; i < A.Height; i++)
						if (A.Entry[i][j0]) {
							A.AddRow(rank, i);
							B.AddRow(rank, i);
						}
					rank++;
					break;
				}
		}
		for (int i = rank; i < A.Height; i++)
			for (int j = 0; j < B.Width; j++)
				if (B.Entry[i][j]) return false;
		return true;
	}

	// Check if kernel is contained in kernel of B.
	bool HasKernelContainedIn(Matrix B) const {
		Matrix A = *this;
		int rank = 0; // will grow
		for (int i0 = 0; i0 < A.Height; i0++) {
			for (int j0 = rank; j0 < A.Width; j0++)
				if (A.Entry[i0][j0]) { // found pivot
					A.ColSwap(j0, rank);
					B.ColSwap(j0, rank);
					for (int j = rank + 1; j < A.Width; j++)
						if (A.Entry[i0][j]) {
							A.AddCol(rank, j);
							B.AddCol(rank, j);
						}
					rank++;
					break;
				}
		}
		for (int j = rank; j < A.Width; j++)
			for (int i = 0; i < B.Height; i++)
				if (B.Entry[i][j]) return false;
		return true;
	}
};

// Class describing a subset of {0,...,n-1} for 1<=n<=MAX_DIM.
// Elements are presented in increasing order.
class Subset {
public:
	int Set;
	int Order;
	int Element[MAX_DIM];

	// Empty set.
	Subset(int n) {
		Set = n;
		Order = 0;
	}

	// Increment subset by replacing it with its successor
	// in the totally ordered sequence of all subsets.
	// Return false if no successor exists.
	bool Inc() {
		for (int i = Order - 1; i >= 0; i--)
			if (Element[i] != Set - Order + i) {
				Element[i]++;
				for (int j = i + 1; j < Order; j++)
					Element[j] = Element[i] + j - i;
				return true;
			}
		if (Order == Set) return false;
		Order++;
		for (int i = 0; i < Order; i++)
			Element[i] = i;
		return true;
	}

	// Convert a subset to string.
	std::string ToString() const {
		std::string s = "";
		for (int i = 0; i < Order; i++) {
			s += std::to_string(Element[i]);
			if (i != Order - 1) s += ",";
		}
		return s;
	}

	// Permutation matrix associated with a subset of n elements of order k.
	// Heights of first k columns are given by the set in increasing order,
	// and the remaining columns form increasing sequence as well.
	// Example: subset {1,3} with n=4 is associated with P=[0010;1000;0001;0100].
	Matrix Permut() const {
		Matrix P(Set);
		P = 0;
		for (int j = 0; j < Order; j++)
			P.Entry[Element[j]][j] = true;
		for (int j = Order; j < Set; j++) {
			int i = j - Order;
			for (int k = 0; k < Order; k++)
				if (Element[k] <= j - Order + k) i++;
			P.Entry[i][j] = true;
		}
		return P;
	}
};

// Given n�n matrix A and 0<p<n, check if there exists idempotent E of rank p in block form
// E=[I0;XI][IY;00][I0;XI] (with the upper left block of size p�p),
// such that (A-E)^3=0. Heights of block X are limited by heights[].
// (E.g. heights[] = {0,0,...,0} means that X = 0 is the only option.)
// If E does not exist, false is returned. Uses p-q algorithm.
bool FindIdempotentXY(Matrix A, int p, int heights[MAX_DIM], Matrix& E) {
	int n = A.Height;
	bool X[MAX_DIM][MAX_DIM] = {}; // block X in the decomposition of E, initialized to 0

	// X runs through all possible cases. In this process, we maintain a number of variables,
	// as explained below. First, we keep track of the counter defining which entry of
	// X is about to change. Initialize the counter to 0.
	bool CounterX[MAX_DIM][MAX_DIM] = {};

	// Second, we completely maintain all the four blocks of the matrix
	// B = [I0;XI]A[I0;XI]+[I0;00]. The blocks shall be denoted Bij.
	// As X=0 at beginning, we initialize the blocks as follows:
	Matrix B = A;
	Matrix B11 = B.SubMatrix(p, p, 0, 0) + 1;
	Matrix B12 = B.SubMatrix(p, n - p, 0, p); // this is actually independent of X
	Matrix B21 = B.SubMatrix(n - p, p, p, 0);
	Matrix B22 = B.SubMatrix(n - p, n - p, p, p);

	// Third, we maintain the following variables (which are actually close to the blocks of B^2):
	Matrix C = B11 * B12 + B12 * B22; // this is independent of X
	Matrix C1 = B11 + B11 * B11 + B12 * B21;
	Matrix C2 = B22 + B22 * B22 + B21 * B12;

	// Fourth, maintain the following:
	Matrix D = B21 * B11 + B22 * B21;
	Matrix D1 = D * B11;
	Matrix D2 = B22 * D;

	// Begin the loop for X. Because we are using the label/goto command, we need to
	// declare a few local variables needed in the loop.
	int q, jmin;
	Matrix U, V, Q11, Q12, Q21, Q22, matQ, matX;

	int nminusp = n - p;
	int pplusq;

	// Begin the loop.
ConsiderX:

	// We need to see if there exists Y such that P = [B11 Y ; B21 B22] satisfies P^3 = 0.
	// First, three important necessary conditions for the existence of Y.
	if (!D2.HasImageContainedIn(B21) || !B21.HasKernelContainedIn(D1)) goto IncreaseX;
	q = B21.Diagonalize(U, V);
	pplusq = p + q;
	if (n > p + q && !(U * B22 * B22 * B22).SubMatrix(n - p - q, n - p, q, 0).
			HasImageContainedIn((U * D).SubMatrix(n - p - q, p, q, 0))) goto IncreaseX;
	if (p > q && !(D * V).SubMatrix(n - p, p - q, 0, q).
			HasKernelContainedIn((B11 * B11 * B11 * V).SubMatrix(p, p - q, 0, q))) goto IncreaseX;
	
	// We conjugate the matrix P by diag(V^(-1), U) such that the lower left corner becomes nicer.
	// We get the following blocks:
	Q11 = V.Inv() * B11 * V;
	Q21 = U * B21 * V; // the nice diagonal block
	Q22 = U * B22 * U.Inv();

	// The upper right block Q12 changes with Y and
	// must always satisfy Q21 Q12 Q21 = Q22^2 Q21 + Q22 Q21 Q11 + Q21 Q11^2
	// (direct computation). This means that the upper right q�q block of Q12 must equal to:
	Q12 = Q22 * Q22 * Q21 + Q22 * Q21 * Q11 + Q21 * Q11 * Q11;

	// Hence Y is not arbitrary. The first two necessary conditions on X guarantee that Q12
	// defined as above actually is zero outside the q�q block,
	// so it does satisfy Q12 = Q21 Q12 Q21.

	// Now we begin another loop where Q12 outside the q�q block runs through all possible cases.
	// Qij are no longer maintained, but we maintain Q = [Qij] and Q^2 (both as an array)
	// and the CounterY for changing the entries of Q21.
	bool Q[MAX_DIM][MAX_DIM];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (i < p && j < p) Q[i][j] = Q11.Entry[i][j];
			else if (i < p) Q[i][j] = Q12.Entry[i][j - p];
			else if (j < p) Q[i][j] = Q21.Entry[i - p][j];
			else Q[i][j] = Q22.Entry[i - p][j - p]; // initializing Q
	bool Q2[MAX_DIM][MAX_DIM];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			bool value = false;
			for (int k = 0; k < n; k++)
				if (Q[i][k] && Q[k][j]) value = !value;
			Q2[i][j] = value;
		} // initializing Q^2
	bool CounterY[MAX_DIM][MAX_DIM];
	for (int i = 0; i < p; i++)
		for (int j = p; j < n; j++)
			CounterY[i][j] = false; // initializing the counter

	// Begin the loop.
ConsiderY:
	for (int i = 0; i != n; i++)
		for (int j = 0; j != n; j++) {
			bool value = false;
			for (int k = 0; k < n; k++)
				if (Q2[i][k] && Q[k][j]) value = !value;
			if (value) goto IncreaseY; // Q^3 is not zero
		}

	// Q^3 is zero! Now retrieve save Q as a matrix,
	// conjugate it back by U and V, and then by [I0;XI], and we are done.
	matQ = Matrix(n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			matQ.Entry[i][j] = Q[i][j];
	matQ = V.Diag(U.Inv()) * matQ * V.Inv().Diag(U);
	matX = Matrix(n);
	matX = 1;
	for (int i = p; i < n; i++)
		for (int j = 0; j < p; j++)
			matX.Entry[i][j] = X[i - p][j]; // matX = [I0;XI]
	E = A + matX * matQ * matX;
	return true;
IncreaseY:

	// We are increasing Y (a.k.a. Q12). Position (i,j) of the bit to be changed is
	// computed using CounterY. Recall that we are skipping the upper left q�q block.
	jmin = pplusq;
	for (int i = 0; i != p; i++) {
		if (i == q) jmin = p;
		for (int j = jmin; j != n; j++)
			if (CounterY[i][j]) CounterY[i][j] = false;
			else {
				CounterY[i][j] = true; // increase the counter

				// Now update all maintained variables in the inner loop: Q and Q2
				for (int k = 0; k < n; k++)
					if (Q[k][i]) Q2[k][j] = !Q2[k][j];
				for (int k = 0; k < n; k++)
					if (Q[j][k]) Q2[i][k] = !Q2[i][k];
				Q[i][j] = !Q[i][j];
				goto ConsiderY;
			}
	}

	// CounterY has been searched through, without finding the entry of Y to change.
	// Hence the inner loop is over, and we proceed by increasing X.
IncreaseX:
	for (int j = 0; j < p; j++)
		for (int i = 0; i < heights[j]; i++)
			if (CounterX[i][j]) CounterX[i][j] = false;
			else {
				CounterX[i][j] = true; // increase the counter

				// Now update all variables maintained in the outer loop: X, Bij, C1, C2, D, D1, D2
				X[i][j] = !X[i][j]; // update X
				for (int k = 0; k < nminusp; k++) {
					if (B12.Entry[j][k]) B22.Entry[i][k] = !B22.Entry[i][k]; // add j-th row of B12 to i-th row of B22
					if (C.Entry[j][k]) C2.Entry[i][k] = !C2.Entry[i][k]; // add j-th row of C to i-th row of C2
				}
				for (int k = 0; k < p; k++) {
					bool value = false;
					for (int l = 0; l < nminusp; l++)
						if (B12.Entry[j][l] && D.Entry[l][k]) value = !value;
					if (value) D2.Entry[i][k] = !D2.Entry[i][k]; // add (j-th row of D12) * D to i-th row of D2 
				}
				for (int k = 0; k < nminusp; k++) {
					bool value = false;
					for (int l = 0; l < nminusp; l++)
						if (B22.Entry[k][l] && C2.Entry[l][i]) value = !value;
					if (value) D2.Entry[k][j] = !D2.Entry[k][j]; // add B22 * (i-th column of C2) to j-th column of D2
				}
				for (int k = 0; k < nminusp; k++)
					if (B22.Entry[k][i])
						for (int l = 0; l < p; l++)
							if (C1.Entry[j][l]) D2.Entry[k][l] = !D2.Entry[k][l]; // add (i-th column of B22) * (j-th row of C1) to D2
				for (int k = 0; k < p; k++) {
					if (B11.Entry[j][k]) B21.Entry[i][k] = !B21.Entry[i][k];
					if (C1.Entry[j][k]) D.Entry[i][k] = !D.Entry[i][k];
				}
				for (int k = 0; k < nminusp; k++) {
					if (B22.Entry[k][i]) B21.Entry[k][j] = !B21.Entry[k][j];
					if (C2.Entry[k][i]) D.Entry[k][j] = !D.Entry[k][j];
				}
				B21.Entry[i][j] = !B21.Entry[i][j]; // update B21
				for (int k = 0; k < nminusp; k++) {
					bool value = false;
					for (int l = 0; l < p; l++)
						if (D.Entry[k][l] && B12.Entry[l][i]) value = !value;
					if (value) D1.Entry[k][j] = !D1.Entry[k][j];
				}
				for (int k = 0; k < nminusp; k++)
					if (C2.Entry[k][i])
						for (int l = 0; l < p; l++)
							if (B11.Entry[j][l]) D1.Entry[k][l] = !D1.Entry[k][l];
				for (int k = 0; k < p; k++) {
					bool value = false;
					for (int l = 0; l < p; l++)
						if (C1.Entry[j][l] && B11.Entry[l][k]) value = !value;
					if (value) D1.Entry[i][k] = !D1.Entry[i][k]; // update D1
				}
				for (int k = 0; k < p; k++) {
					if (B12.Entry[k][i]) B11.Entry[k][j] = !B11.Entry[k][j];
					if (C.Entry[k][i]) C1.Entry[k][j] = !C1.Entry[k][j];
				}
				goto ConsiderX;
			}

	// CounterX has been searched through, without success. Hence E does not exist.
	return false;
}

// Given n�n matrix A, find idempotent E such that (A-E)^3=0. If showsteps is chosen,
// steps are written. Return false if E does not exist.
bool FindIdempotent(Matrix A, bool showsteps, Matrix& E) {
	int n = A.Height;
	if (A * A * A == 0) {
		E = Matrix(n);
		E = 0;
		return true;
	}
	Matrix Q = A + 1;
	if (Q * Q * Q == 0) {
		E = Matrix(n);
		E = 1;
		return true;
	} // trivial cases
	Subset M(n);
	while (M.Inc()) {
		if (M.Order == n || ((M.Order % 2 == 1) ^ A.Trace())) continue; // trace(A) = trace(E)
		if (showsteps) std::cout << "Set = " << M.ToString() << "\n";
		Matrix P = M.Permut();
		int heights[MAX_DIM];
		for (int j = 0; j < M.Order; j++)
			heights[j] = M.Element[j] - j;
		if (FindIdempotentXY(P.Trans() * A * P, M.Order, heights, E)) {
			E = P * E * P.Trans();
			return true;
		}
	}
	return false;
}

// Companion matrix with given last column.
Matrix Companion(std::string column) {
	int n = column.length();
	Matrix A(n);
	A = 0;
	for (int i = 1; i < n; i++)
		A.Entry[i][i - 1] = true;
	for (int i = 0; i < n; i++)
		A.Entry[i][n - 1] = (column[i] == '1');
	return A;
}

int main() {
	std::cout << "Nil-Clean, author Janez Ster, March 2020\n\n";
	std::cout << "A = diag(comp_1, comp_2, ..., comp_n)";
	while (true) {
		int n;
		std::cout << "\nNumber of companion matrices (0 for exit): ";
		std::cin >> n;
		if (n == 0) return 0;
		Matrix A;
		int dim = 0;
		for (int i = 0; i < n; i++) {
			std::cout << "Last column of companion matrix no. " << i + 1 << ": ";
			std::string s;
			std::cin >> s;
			dim += s.length();
			if (dim > MAX_DIM) {
				std::cout << "Too big matrix A!\n";
				break;
			}
			if (i == 0) A = Companion(s);
			else A = A.Diag(Companion(s));
		}
		if (dim > MAX_DIM) continue;
		std::cout << "Write down steps (y/n): ";
		std::string s;
		std::cin >> s;
		bool showsteps = (s == "y");
		Matrix E;
		time_t begin = time(0);
		bool found = FindIdempotent(A, showsteps, E);
		time_t end = time(0);
		std::cout << "Finished in " << difftime(end, begin) << " sec.\n";
		if (found) {
			std::cout << "Found E = " << E.ToString() << std::endl;
			std::cout << "Checking ...";
			Matrix Q = A + E;
			if (E * E == E && Q * Q * Q == 0)
				std::cout << " Done.";
			else
				std::cout << " FAILED!!! Bug in the algorithm!";
		}
		else
			std::cout << "Idempotent does not exist.";
		std::cout << std::endl;
	}
}