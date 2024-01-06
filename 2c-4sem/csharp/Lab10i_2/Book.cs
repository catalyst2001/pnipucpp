using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab10_2
{
    public class Book
    {
        public string Title { get; }
        public string Publisher { get; }
        public int Year { get; }

        public Book(string title, string publisher, int year)
            => (Title, Publisher, Year) = (title, publisher, year);
    }
}
