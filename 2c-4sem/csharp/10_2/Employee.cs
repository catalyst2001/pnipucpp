using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _10_2
{
    public class Employee
    {
        private string name;
        private int stage;
        public Employee(string emp_name, int stage)
        {
            this.name = emp_name;
            this.stage = stage;
        }

        public string GetName() { return name; }
        public int GetStage() { return stage; }
    }
}
