using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace euler480n
{
    class Program
    {
        private const int LengthLimit = 15;
        private const string Phrase = "thereisasyetinsufficientdataforameaningfulanswer";
        private static readonly char[] DistinctChars = Phrase.Distinct().OrderBy(c => c).ToArray();
        private static readonly ConcurrentDictionary<CacheKey, long> StartingWithCache = new ConcurrentDictionary<CacheKey, long>();
        private static readonly ConcurrentDictionary<string, long> WordCache = new ConcurrentDictionary<string, long>();

        private static long StartingWith(string word, ICollection<char> remaining)
        {
            var cacheKey = new CacheKey(word, remaining);
            return StartingWithCache.GetOrAdd(cacheKey, CalculateStartingWith);
        }

        private static long CalculateStartingWith(CacheKey k)
        {
            long res = 1;
            if (k.Word.Length < LengthLimit)
            {
                foreach (var c in k.Remaining.Distinct().ToArray())
                {
                    if (k.Remaining.Remove(c))
                    {
                        res += StartingWith(k.Word + c, k.Remaining);
                        k.Remaining.Add(c);
                    }
                }
            }
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

        private static long CalcP(string word)
        {
            long result = word.Length;
            var prefixes = word.SelectMany((c, i)
                => DistinctChars.Where(oc => oc < c).ToArray().Select(oc
                    => word.Substring(0, i) + oc)).OrderBy(s => s).ToArray();
            result += prefixes.AsParallel().Sum(s => StartingWith(s));
            return result;
        }

        private static long P(string word)
        {
            var result = WordCache.GetOrAdd(word, CalcP);
            return result;
        }

        private static void Wr(long n, StringBuilder sb)
        {
            var firstIndexGreater = Array.FindIndex(DistinctChars, i => P(sb.ToString() + i) > n);
            if (firstIndexGreater > 0)
            {
                sb.Append(DistinctChars[firstIndexGreater - 1]);
                Wr(n, sb);
            }
        }

        private static string W(long n)
        {
            var sb = new StringBuilder();
            Wr(n, sb);
            return sb.ToString();
        }

        static void Main(string[] args)
        {
            Console.WriteLine(
                W(P("legionary") + P("calorimeters") - P("annihilate") + P("orchestrated") - P("fluttering")));
        }
    }

    public class CacheKey
    {
        public string Word { get; }
        public ICollection<char> Remaining { get; }
        public string RemainingKey { get; }

        public CacheKey(string word, ICollection<char> remaining)
        {
            Word = word;
            Remaining = remaining;
            RemainingKey = remaining.GroupBy(c => c).Select(g => g.Count()).OrderBy(c => c)
                .Aggregate(new StringBuilder(), (sb, i) => sb.Append(i)).ToString();
        }

        public override bool Equals(object obj)
        {
            return obj is CacheKey key &&
                   RemainingKey == key.RemainingKey;
        }

        public override int GetHashCode()
        {
            return -1573574710 + EqualityComparer<string>.Default.GetHashCode(RemainingKey);
        }
    }

}
