namespace Lab10_1
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

            Console.WriteLine();
            IRandomInit[] randomArray = {
                new State("Судан", "Военная диктатура"),
                new Republic("Молдова"),
                new Monarchy("Великобритания"),
                new Kingdom("Нарния"),
                new SimpleRandom(),
            };

            Array.Sort(randomArray);
            var resultIndex = Array.BinarySearch(randomArray, randomArray[2]);
            Console.WriteLine("Великобритания находится под индексом " + resultIndex);

            Console.WriteLine();
            foreach (var randomClass in randomArray)
            {
                randomClass.RandomInit();
                Console.WriteLine(randomClass.random);
            }

            Console.WriteLine();
            var cloneFullKingdom = (State)states[3].Clone();
            var cloneShallowKingdom = (State)states[3].ShallowCopy();

            Console.WriteLine(cloneFullKingdom.GetName() + " = " + cloneShallowKingdom.GetName());
        }
    }
}