using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab10_2
{
    public class SchoolBook: Book
    {
        public string Discipline { get; }
        
        public SchoolBook(string title, string publisher, int year, string discipline) : base (title, publisher, year)
        {
            this.Discipline = discipline;
        }
    }
}
