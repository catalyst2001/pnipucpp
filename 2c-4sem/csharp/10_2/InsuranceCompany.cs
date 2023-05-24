using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _10_2
{
    internal class InsuranceCompany : Organization
    {
        protected int clients_number;
        public InsuranceCompany(string name, string address, string worktime, int clients) : base(name, address, worktime)
        {
            clients_number = clients;
        }

        public int GetNumClients() { return clients_number; }
        public override string GetFullInfo()
        {
            return $"InsuranceCompany: {name}, {address}, {worktime}. Clients({clients_number})";
        }
        public override Employee[] GetEmployes()
        {
            return new Employee[] {
                new Employee("Виктор", 3),
                new Employee("Мансур", 9),
                new Employee("Алена", 3),
                new Employee("Инна", 5),
                new Employee("Юлия", 2)
            };
        }
    }
}
