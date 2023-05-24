namespace _10_3
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Organization[] organizations = new Organization[5] {
                new Organization("Организация1", "ул. Петрова 44/2", "10:00-22:00"),
                new Factory("Системы отопления", "ул. Аносова 2", "09:00-20:00", 12000, 10000),
                new InsuranceCompany("Страховое АО ВСК", "ул. 25 Октября", "09:00–18:00", 254003),
                new ShipbuildingCompany("Hanse LLC", "10600 1349/127 Charoennakorn Road", "10AM-10PM", 18000, 6000, 24000, 2400),
                new Library("Библиотека", "ул. Смирнова", "12:00-18:00", 1400)
            };

            for (int i = 0; i < organizations.Length; i++)
                Console.WriteLine("{0}: {1}", i, organizations[i].GetFullInfo());
        }
    }
}