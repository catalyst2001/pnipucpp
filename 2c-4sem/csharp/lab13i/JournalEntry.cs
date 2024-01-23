using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab13i
{
    public class JournalEntry
    {
        string NameCollection;
        string EventName;
        string ObjName;

        public JournalEntry(string name, string eventname, string objName) {
            NameCollection = name;
            EventName = eventname;
            ObjName = objName;
        }
    }
}
