using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace lab11k
{
    public class Library : Organization
    {
        private int number_books;
        public Library() : base("", "", "")
        {
            number_books = 0;
            subobject = new Organization("", "", "");
        }

        public Library(string name, string address, string worktime, int num_books) : base(name, address, worktime)
        {
            number_books = num_books;
            subobject = new Organization(name, address, worktime);
        }

        public int GetNumBooks() { return number_books; }
        public void SetNumBooks(int n) { number_books = n; }

        public new string GetFullInfo()
        {
            return $"Library: {name}, {address}, {worktime}. Number of books: {number_books}";
        }

        public new void FillInfo()
        {
            base.FillInfo();
            number_books = (int)ReadLineWithDescription(FORMAT_TYPE.FORMAT_INT32, "Enter number of books");
        }
    }
}
