using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _10_3
{
    public class Organization
    {
        protected string name;
        protected string address;
        protected string worktime;

        public Organization(string _name, string _address, string _worktime)
        {
            name = _name;
            address = _address;
            worktime = _worktime;
        }

        public virtual string GetFullInfo() { return $"{name}, {address}, {worktime}"; }
    }
}
