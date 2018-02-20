using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Mpir.NET;

namespace _0104
{
    class Program
    {
        static mpz_t m = new mpz_t(10).Power(9);
        static bool IsPandigitalStart(mpz_t n)
        {
            return (new string(n.ToString().Substring(0,9).OrderBy(c => c).ToArray()) == "123456789");
        }

        static bool IsPandigitalEnd(mpz_t n)
        {
            return (new string(n.Mod(m).ToString().OrderBy(c => c).ToArray()) == "123456789");
        }
        static void Main(string[] args)
        {
            var queue = new Queue<mpz_t>(new mpz_t[] { 1, 1 });
            int i = 2;
            while (true)
            {
                var next = queue.Dequeue() + queue.Peek();
                ++i;
                if(IsPandigitalEnd(next))
                {
                    if(IsPandigitalStart(next))
                    {
                        Console.WriteLine($"Start! {i}");
                    }
                }
                queue.Enqueue(next);
            }
        }
    }
}
