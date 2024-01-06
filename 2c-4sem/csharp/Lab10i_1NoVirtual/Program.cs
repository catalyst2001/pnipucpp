namespace Lab10_1NoVirtual
{
    public class Program
    {
        static void Main(string[] args)
        {
            State[] states = {
                new State("Судан", "Военная диктатура"),
                new Republic("Молдова"),
                new Monarchy("Великобритания"),
                new Kingdom("Нарния"),
            };

            Console.WriteLine("States type:");
            foreach (var state in states)
                Console.WriteLine(state.GetType());
            Console.WriteLine();

            Console.WriteLine("IS State:");
            foreach (var state in states)
                Console.WriteLine(state is State);
            Console.WriteLine();

            Console.WriteLine("IS Monarchy:");
            foreach (var state in states)
                Console.WriteLine(state is Monarchy);

            Console.WriteLine();
            foreach (var state in states)
            {
                Console.WriteLine(state.GetName() + " - " + state.GetGovernmentForm());
            }
        }
    }
}