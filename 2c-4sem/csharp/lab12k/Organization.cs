using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab12k
{
    public class Organization : ConInFormat, IComparable<Organization>
    {
        protected string name;
        protected string address;
        protected string worktime;
        protected Organization subobject;

        //TODO: lab11 part 3  w 1
        public Organization Base {
            get {
                return new Organization(name, address, worktime);
            }
        }

        public Organization GetSubobject() { return subobject; }

        public Organization() { name = ""; address = ""; worktime = ""; }

        public Organization(string _name, string _address, string _worktime)
        {
            name = _name;
            address = _address;
            worktime = _worktime;
        }

        public string GetOrgName() { return name; }

        public string GetFullInfo() { return $"{name}, {address}, {worktime} --"; }

        public virtual void FillInfo()
        {
            name = (string)ReadLineWithDescription(FORMAT_TYPE.FORMAT_STRING, "Enter name");
            address = (string)ReadLineWithDescription(FORMAT_TYPE.FORMAT_STRING, "Enter address");
            worktime = (string)ReadLineWithDescription(FORMAT_TYPE.FORMAT_STRING, "Enter work time");
        }

        /* comparer */
        public int CompareTo(Organization? other)
        {
            return other.ToString().CompareTo(ToString());
        }
    }
}
