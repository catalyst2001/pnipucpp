using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _10_3
{
    public class InsuranceCompany : Organization
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
    }
}
