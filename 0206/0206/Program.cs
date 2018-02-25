using System;
using System.Text.RegularExpressions;

namespace _0206
{
    class Program
    {
        const string pattern = @"1_2_3_4_5_6_7_8_9_0";
        static readonly Regex r = new Regex(pattern.Replace("_", @"\d"), RegexOptions.Compiled);
        static bool IsSquare(long l, out long sqrt)
        {
            sqrt = (long)Math.Sqrt(l);
            return sqrt * sqrt == l;
        }

        static bool Search(char[] c, int n)
        {
            for(char t = '0'; t <= '9'; t++)
            {
                c[n] = t;
                if(n == 1)
                {
                    long num = long.Parse(new string(c));
                    if (IsSquare(num, out long sqrt))
                    {
                        Console.WriteLine(num);
                        Console.WriteLine(sqrt);
                        return true;
                    }
                }
                else
                {
                    if (Search(c, n - 2))
                        return true;
                }
            }
            return false;
        }


        static void Main(string[] args)
        {
            var chars = pattern.ToCharArray();
            Search(chars, Array.LastIndexOf(chars, '_'));
        }
    }
}
