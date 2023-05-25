using Microsoft.VisualStudio.TestPlatform.TestHost;

namespace _10_3
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void BinSearch_Test()
        {
            Organization[] array = new Organization[] {
                new Factory("Системы отопления", "ул. Аносова 2", "09:00-20:00", 12000, 10000),
                new InsuranceCompany("Страховое АО ВСК", "ул. 25 Октября", "09:00–18:00", 254003),
                new ShipbuildingCompany("Hanse LLC", "10600 1349/127 Charoennakorn Road", "10AM-10PM", 18000, 6000, 24000, 2400),
                new Library("Библиотека", "ул. Смирнова", "12:00-18:00", 1400),
                new Library("Библиотека 2", "ул. Смирнова 2", "12:00-18:00", 14400),
            };

            int search_val = array[2].GetOrganizationId();
            int idx = Program.BinSearch(array, search_val);
            Assert.IsTrue(array[idx].GetOrganizationId() == search_val);
        }
    }
}