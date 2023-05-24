using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _10_2
{
    public class Factory : Organization
    {
        protected int done_plan;
        protected int total_plan;
        public Factory(string name, string address, string worktime, int plan, int done_plan) : base(name, address, worktime)
        {
            this.total_plan = plan;
            this.done_plan = done_plan;
        }

        public int getPlan() { return total_plan; }
        public void setPlan(int plan) { this.total_plan = plan; }
        public int getDonePlan() {  return done_plan; }
        public void setDonePlan(int plan) { done_plan = plan; }

        public override string GetFullInfo()
        {
            return $"Factory: {name}, {address}, {worktime}. (Done|Total) All({done_plan}|{total_plan})";
        }
        public override Employee[] GetEmployes()
        {
            return new Employee[] {
                new Employee("Виктор", 23),
                new Employee("Сергей", 3),
                new Employee("Сергей", 6),
                new Employee("Усмон", 15),
                new Employee("Захар", 12),
                new Employee("Ольга", 10),
                new Employee("Евгения", 9),
                new Employee("Леонид", 4),
                new Employee("Андрей", 2)
            };
        }
    }
}
