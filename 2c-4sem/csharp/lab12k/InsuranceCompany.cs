using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab12k
{
    public class InsuranceCompany : Organization
    {
        protected int clients_number;
        public InsuranceCompany() : base("", "", "")
        {
            clients_number = 0;
            subobject = new Organization("", "", "");
        }

        public InsuranceCompany(string name, string address, string worktime, int clients) : base(name, address, worktime)
        {
            clients_number = clients;
            subobject = new Organization(name, address, worktime);
        }

        public int GetNumClients() { return clients_number; }
        public new string GetFullInfo()
        {
            return $"InsuranceCompany: {name}, {address}, {worktime}. Clients({clients_number})";
        }

        public override void FillInfo()
        {
            base.FillInfo();
            clients_number = (int)ReadLineWithDescription(FORMAT_TYPE.FORMAT_INT32, "type clients number");
        }
    }
}
