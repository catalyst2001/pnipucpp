namespace _10_2
{
    internal class Program
    {
        static void PrintEmployersWithGreaterStage(int stage, ref Organization[] organizations)
        {
            Console.WriteLine("--- вывод имен служащих со стажем не меньше {0} ---", stage);
            foreach (Organization organization in organizations)
                foreach (Employee emp in organization.GetEmployes())
                    if(emp.GetStage() >= stage)
                        Console.WriteLine("  {0}", emp.GetName());

            Console.WriteLine();
        }

        static void Main(string[] args)
        {
            Organization[] organizations = new Organization[] {
                new Factory("Системы отопления", "ул. Аносова 2", "09:00-20:00", 12000, 10000),
                new InsuranceCompany("Страховое АО ВСК", "ул. 25 Октября", "09:00–18:00", 254003),
                new ShipbuildingCompany("Hanse LLC", "10600 1349/127 Charoennakorn Road", "10AM-10PM", 18000, 6000, 24000, 2400),
                new Library("Библиотека", "ул. Смирнова", "12:00-18:00", 1400)
            };

            Console.WriteLine("--- список организаций ---");
            for (int i = 0; i < organizations.Length; i++)
                Console.WriteLine("{0}: {1}", i, organizations[i].GetFullInfo());

            try
            {
                do
                {
                    Console.Write("Введите стаж: ");
                    int stage_val = Convert.ToInt32(Console.ReadLine());
                    PrintEmployersWithGreaterStage(stage_val, ref organizations);
                } while (true);
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
        }
    }
}