#include <iostream>
#include <string>
using namespace std;

// Struktur Buku
struct Buku
{
    string judul, pengarang;
    int tahun;
    Buku *left;
    Buku *right;

    Buku(string j, string p, int t) : judul(j), pengarang(p), tahun(t), left(NULL), right(NULL) {}
};

// Stack manual
struct StackNode
{
    string aksi; // "tambah" atau "hapus"
    Buku *data;
    StackNode *next;
};

StackNode *top = NULL;

void push(string aksi, Buku *buku)
{
    StackNode *newNode = new StackNode{aksi, new Buku(*buku), top};
    top = newNode;
}

void pop()
{
    if (top)
    {
        StackNode *temp = top;
        top = top->next;
        delete temp->data;
        delete temp;
    }
}

bool isEmpty()
{
    return top == NULL;
}

StackNode *peek()
{
    return top;
}

// Fungsi BST
Buku *insertBuku(Buku *root, Buku *baru)
{
    if (!root)
        return baru;
    if (baru->judul < root->judul)
        root->left = insertBuku(root->left, baru);
    else
        root->right = insertBuku(root->right, baru);
    return root;
}

void inOrder(Buku *root)
{
    if (root)
    {
        inOrder(root->left);
        cout << "Judul      : " << root->judul << endl;
        cout << "Pengarang  : " << root->pengarang << endl;
        cout << "Tahun      : " << root->tahun << endl;
        cout << "----------------------------------\n";
        inOrder(root->right);
    }
}

Buku *findMin(Buku *node)
{
    while (node->left)
        node = node->left;
    return node;
}

Buku *deleteBuku(Buku *root, string judul, bool &found, Buku *&deletedNode)
{
    if (!root)
        return NULL;
    if (judul < root->judul)
        root->left = deleteBuku(root->left, judul, found, deletedNode);
    else if (judul > root->judul)
        root->right = deleteBuku(root->right, judul, found, deletedNode);
    else
    {
        found = true;
        deletedNode = new Buku(*root);
        deletedNode->left = NULL;
        deletedNode->right = NULL;
        if (!root->left)
        {
            Buku *temp = root->right;
            delete root;
            return temp;
        }
        else if (!root->right)
        {
            Buku *temp = root->left;
            delete root;
            return temp;
        }
        else
        {
            Buku *temp = findMin(root->right);
            root->judul = temp->judul;
            root->pengarang = temp->pengarang;
            root->tahun = temp->tahun;
            root->right = deleteBuku(root->right, temp->judul, found, deletedNode);
        }
    }
    return root;
}

Buku *undo(Buku *root)
{
    if (isEmpty())
    {
        cout << "Tidak ada aksi untuk di undo.\n";
        return root;
    }

    StackNode *last = peek();
    if (last->aksi == "tambah")
    {
        bool dummy;
        Buku *dummyNode = NULL;
        root = deleteBuku(root, last->data->judul, dummy, dummyNode);
        cout << "Buku berjudul \"" << last->data->judul << "\" batal ditambahkan!\n";
    }
    else if (last->aksi == "hapus")
    {
        Buku *bukuUndo = new Buku(*last->data);
        bukuUndo->left = NULL;
        bukuUndo->right = NULL;
        root = insertBuku(root, bukuUndo);
        cout << "Buku berjudul \"" << last->data->judul << "\" batal dihapus!\n";
    }

    pop();
    return root;
}

int main()
{
    Buku *root = NULL;
    int pilihan;

    do
    {
        cout << "\n===== Pilihan Menu BookTrack =====\n";
        cout << "1. Tambah Buku\n";
        cout << "2. Tampilkan Buku (A-Z)\n";
        cout << "3. Hapus Buku (berdasarkan judul)\n";
        cout << "4. Undo Aksi Terakhir\n";
        cout << "5. Keluar\n";
        cout << "Pilih : ";
        cin >> pilihan;
        cin.ignore();

        system("cls");

        if (pilihan == 1)
        {
            string judulBuku, pengarangBuku;
            int tahunBuku;
            cout << "Judul      : ";
            getline(cin, judulBuku);
            cout << "Pengarang  : ";
            getline(cin, pengarangBuku);
            cout << "Tahun      : ";
            cin >> tahunBuku;
            Buku *bukuBaru = new Buku(judulBuku, pengarangBuku, tahunBuku);
            root = insertBuku(root, bukuBaru);
            push("tambah", bukuBaru);
            cout << "Selamat!!! Buku berjudul \"" << bukuBaru->judul << "\" berhasil ditambahkan.\n";
        }
        else if (pilihan == 2)
        {
            cout << "\nDaftar Buku!\n\n";
            inOrder(root);
        }
        else if (pilihan == 3)
        {
            cout << "\nDaftar Buku Saat Ini!\n\n";
            inOrder(root);

            string j;
            cout << "\nMasukkan judul buku yang akan dihapus : ";
            getline(cin, j);
            bool found = false;
            Buku *deleted = NULL;
            root = deleteBuku(root, j, found, deleted);
            if (found)
            {
                push("hapus", deleted);
                cout << "\nBuku berjudul \"" << deleted->judul << "\" berhasil dihapus.\n";
            }
            else
            {
                cout << "Buku berjudul \"" << j << "\" tidak ditemukan.\n";
            }
        }
        else if (pilihan == 4)
        {
            root = undo(root);
        }
        else if (pilihan == 5)
        {
            cout << "Terima kasih Telah Menggunakan Program Ini!\n";
        }
        else
        {
            cout << "Pilihan menu tidak valid.\n";
        }

    } while (pilihan != 5);

    return 0;
}