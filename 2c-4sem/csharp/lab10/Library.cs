using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab10
{
    public class Library : Organization
    {
        private int number_books;
        public Library(string name, string address, string worktime, int num_books) : base(name, address, worktime)
        {
            number_books = num_books;
        }

        public int GetNumBooks() { return number_books; }
        public void SetNumBooks(int n) { number_books = n; }

        public new string GetFullInfo()
        {
            return $"Library: {name}, {address}, {worktime}. Number of books: {number_books}";
        }
    }
}
