using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _10_3
{
    public class Organization : IRandomInit, IComparable<Organization>, ICloneable
    {
        protected int orgid;
        protected string name;
        protected string address;
        protected string worktime;

        public object Clone()
        {
#if DEPTH_CLONE
            Organization org = new Organization(name, address, worktime); //глубокое клонирование
            org.orgid = orgid;
            return org;
#else
            return MemberwiseClone(); // поверхностное копирование.
#endif
        }

        public int CompareTo(Organization other) { return orgid.CompareTo(other.orgid); }

        public void RandomInit()
        {
            Random random = new Random();
            int start_hour = random.Next(8, 13);
            int start_minute = random.Next(0, 59);
            worktime = $"{start_hour}:{start_minute}-{(start_hour + random.Next(5, 12)) % 24}:{start_minute}";
        }

        public Organization(string _name, string _address, string _worktime)
        {
            name = _name;
            address = _address;
            worktime = _worktime;
            orgid = new Random().Next(1000, 100000);
        }

        public int GetOrganizationId() { return orgid; }

        public virtual string GetFullInfo() { return $"{name}, {address}, {worktime}"; }
    }
}
