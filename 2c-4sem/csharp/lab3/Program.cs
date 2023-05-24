namespace lab3
{
    public class Program
    {
        static void Main(string[] args)
        {
            double a = 0.1;
            double b = 0.8;
            double e = 0.0001;
            double k = 10.0;
            double n = 10.0;
            for (double x = a; x <= b; x += (b - a) / k)
            {
                Console.WriteLine(
                    "X = {0:F10}   SN = {1:F10}  SE = {2:F10}  Y = {3:F10}",
                    x, Functions.ComputeSN(x, n), Functions.ComputeSE(x, e), Functions.ComputeY(x)
                );
            }
        }
    }
}