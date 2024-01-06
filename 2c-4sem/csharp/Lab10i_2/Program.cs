namespace Lab10_2
{
    internal class Program
    {
        static void Main(string[] args)
        {
            var books = new List<Book>();
            books.Add(new Book("Сказки", "ООО Книжный", 2000));
            books.Add(new SchoolBook("История России", "ООО Книжный", 2000, "История"));
            books.Add(new Book("Мемуары Великих людей", "ООО Книжный", 2010));
            books.Add(new SchoolBook("Математика для 7 класса", "ООО Книжный", 2001, "Математика"));
            books.Add(new Book("Легенды Древней Греции", "ООО Книжный", 2012));

            Console.WriteLine("Для вывода наименований всех книг в библиотеке (магазине), вышедших не ранее указанного года, введите год: ");
            var year = int.Parse(Console.ReadLine());
            foreach (Book book in books)
            {
                if (book.Year > year)
                    Console.WriteLine(book.Title);
            }

            Console.WriteLine("\nДля вывода наименований всех учебников в библиотеке (магазине), введите дисциплину: ");
            var discipline = Console.ReadLine();
            foreach (var book in books)
            {
                if (book is SchoolBook && ((SchoolBook)book).Discipline == discipline)
                    Console.WriteLine(book.Title);
            }

            Console.WriteLine("\nДля вывода количества книг в библиотеке (магазине), за указанный год, введите год: ");
            year = int.Parse(Console.ReadLine());
            int count = 0;
            foreach (Book book in books)
            {
                if (book.Year == year)
                    count++;
            }
            Console.WriteLine("Найдено: " + count + " шт.");
        }
    }
}