using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace printfactors
{
    class Program
    {
        static void Main(string[] args)
        {

            string line;
            int i;
            while ((line = Console.ReadLine()) != null && int.TryParse(line, out i))
            {
                for(int f = 2; f < i; f++)
                    if(IsIntegral((double)i/f)) Console.Write(f + "   ");
                Console.WriteLine();
            }
        }

        public static bool IsIntegral(double d)
        {
            return Math.Abs(d - Math.Round(d, 0)) < 1e-9;
        }


    }
}
