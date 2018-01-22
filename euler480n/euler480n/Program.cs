using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace euler480n
{
    class Program
    {
        private const int LengthLimit = 15;
        private const string Phrase = "thereisasyetinsufficientdataforameaningfulanswer";
        //private const int LengthLimit = 4;
        //private const string Phrase = "aabc";
        /*
         * 15 : aaaaaacdeeeeeef
           16 : aaaaaacdeeeeeeg
           17 : aaaaaacdeeeeeeh
*/

        private static readonly Dictionary<string, long> StartingWithCache = new Dictionary<string, long>();

        static long StartingWith(string word, List<char> remaining)
        {
            var key = remaining.GroupBy(c => c).Select(g => g.Count()).OrderBy(c => c)
                .Aggregate(new StringBuilder(), (sb, i) => sb.Append(i)).ToString();
            if (!StartingWithCache.TryGetValue(key, out var res))
            {
                res = 1;
                if(word.Length < LengthLimit)
                {
                    foreach (var c in remaining.Distinct().ToArray())
                    {
                        if (remaining.Remove(c))
                        {
                            res += StartingWith(word + c, remaining);
                            remaining.Add(c);
                        }
                    }
                }
                StartingWithCache.Add(key, res);
            }
            //Console.WriteLine($"{res} starting with {word}");
            return res;
        }

        private static long StartingWith(string word)
        {
            var remaining = Phrase.ToList();
            foreach (var c in word)
            {
                if (!remaining.Remove(c)) return 0;
            }
            return StartingWith(word, remaining);
        }

        static long N(string word)
        {
            long result = 0;
            for (int pos = word.Length - 1; pos >= 0; pos--)
            {
                var prefix = word.Substring(0, pos);
                result++;
                //Console.WriteLine($"1 for {prefix}");
                foreach (var c in Phrase.Distinct().Where(c => c < word[pos]).ToArray())
                {
                    result += StartingWith(prefix + c);
                }
             }
            return result;
        }

        private static void test(string word)
        {
            Console.WriteLine($"N({word}) = {N(word)}");
        }

        static void Main(string[] args)
        {
            Console.Out.WriteLine(N("aaaaaacdeeeeeef"));
            Console.Out.WriteLine(N("aaaaaacdeeeeeey"));
            Console.Out.WriteLine(N("euleoywuttttsss"));
            Console.Out.WriteLine(N("euler"));
        }
    }
}
