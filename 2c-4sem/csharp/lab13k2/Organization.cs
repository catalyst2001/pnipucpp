using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace lab13k2
{
    public class Organization : IComparable<Organization>
    {
        protected string name;
        protected string address;
        protected string worktime;

        protected void FillDefault(string prefix)
        {
            Random rand = new Random();
            name = $"{prefix}_{rand.Next()}";
            address = $"address {rand.Next(0, 2000)}";
            int startHour = rand.Next(8, 12);
            int startMins = rand.Next(0, 30);
            int endHour = rand.Next(startHour + 1, startHour + 8) % 24;
            int endMins = rand.Next(0, 30);
            worktime = $"{startHour}:{startMins}-{endHour}:{endMins}";
        }

        public Organization()
        {
            name = "empty";
            address = "empty";
            worktime = "empty";
        }
        public int CompareTo(Organization? other)
        {
            return name.CompareTo(other.name);
        }

        public Organization(string _name, string _address, string _worktime)
        {
            name = _name;
            address = _address;
            worktime = _worktime;
        }

        public string GetFullInfo() { return $"{name}, {address}, {worktime} --"; }
        public void AutoFill()
        {
            FillDefault("Organization");
        }

        public override string ToString()
        {
            return GetFullInfo();
        }
    }
}
