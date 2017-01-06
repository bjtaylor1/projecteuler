using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;

namespace euler579
{
    public class DatabaseHelper : IDisposable
    {
        private SqlConnection sqlConnection;

        public DatabaseHelper()
        {
            sqlConnection = new SqlConnection("Initial Catalog=triples;Integrated Security=SSPI;MultipleActiveResultSets=true");
            sqlConnection.Open();
        }

        public Vector3D[] GetAllPrimitivesOfSameSquare(Vector3D vector)
        {
            var vectors = new List<Vector3D>();
            using (var cmd = new SqlCommand("select A,B,C from primitivetriples where [square] = @square", sqlConnection))
            {
                cmd.Parameters.AddWithValue("@square", vector.Length);
                using (var rs = cmd.ExecuteReader())
                {
                    while (rs.Read())
                    {
                        var a = (int)rs["A"];
                        var b = (int)rs["B"];
                        var c = (int)rs["C"];
                        var v = new Vector3D(a, b, c);
                        if (!vector.Equals(v)) vectors.Add(v);
                    }
                }
            }
            return vectors.ToArray();
        }

        public void ResetDone()
        {
            using (var cmd = new SqlCommand("delete DoneTriples", sqlConnection))
            {
                cmd.ExecuteNonQuery();
            }
        }

        public bool IsDone(int[] vector)
        {
            using (var cmd = new SqlCommand(@"select count(*) from DoneTriples where A = @A and B = @B and C = @C", sqlConnection))
            {
                cmd.Parameters.AddWithValue("@A", vector[0]);
                cmd.Parameters.AddWithValue("@B", vector[1]);
                cmd.Parameters.AddWithValue("@C", vector[2]);
                var count = Convert.ToInt32(cmd.ExecuteScalar());
                var isDone = count > 0;
                return isDone;
            }
        }

        public void SetDone(int[] vector)
        {
            using (var cmd = new SqlCommand(@"insert DoneTriples(A,B,C) values(@A,@B,@C)", sqlConnection))
            {
                cmd.Parameters.AddWithValue("@A", vector[0]);
                cmd.Parameters.AddWithValue("@B", vector[1]);
                cmd.Parameters.AddWithValue("@C", vector[2]);
                cmd.ExecuteNonQuery();
            }
        }

        public static DatabaseHelper Instance { get; } = new DatabaseHelper();
        public void Dispose()
        {
            sqlConnection.Dispose();
        }

        public void FindTwins()
        {
            using (var cmd = new SqlCommand("delete primitivetripletwins", sqlConnection))
            {
                cmd.ExecuteNonQuery();
            }
            double count = 0, tot = 853831;
            var twinFields = new string[] {"ux", "uy", "uz", "vx", "vy", "vz", "nx", "ny", "nz"};
            var twinFieldParamNames = twinFields.Select(s => "@" + s).ToArray();
            using (var cmd = new SqlCommand("select A,B,C,[Square] from primitivetriples", sqlConnection))
            using (var cmdAlt = new SqlCommand(
                    "select a,b,c " +
                    "from primitivetriplepermutations " +
                    "where [square] = @square", sqlConnection))
            using(var cmdIns = new SqlCommand($"insert primitivetripletwins({twinFields.ToCsvString()}) values({twinFieldParamNames.ToCsvString()})", sqlConnection))
            {
                var altSqParam = cmdAlt.Parameters.Add("@square", SqlDbType.Int);
                var twin = twinFieldParamNames.ToDictionary(s => s, s => cmdIns.Parameters.Add(s, SqlDbType.Int));

                using (var rs = cmd.ExecuteReader())
                    while (rs.Read())
                    {
                        Console.Write($"\r{count++/tot:0.00%}   ");
                        var a = Convert.ToInt32(rs["A"]);
                        var b = Convert.ToInt32(rs["B"]);
                        var c = Convert.ToInt32(rs["C"]);
                        var square = Convert.ToInt32(rs["square"]);
                        var v1 = new Vector3D(a, b, c);
                        altSqParam.Value = square;
                        bool found = false;
                        using (var rsAlt = cmdAlt.ExecuteReader())
                            while (!found && rsAlt.Read())
                            {
                                var altA = Convert.ToInt32(rsAlt["A"]);
                                var altB = Convert.ToInt32(rsAlt["B"]);
                                var altC = Convert.ToInt32(rsAlt["C"]);
                                var v2 = new Vector3D(altA, altB, altC);
                                if (v1.IsAtRightAnglesTo(v2))
                                {
                                    var cp = Vector3D.CrossProduct(v1, v2);
                                    cp *= (v1.Length / cp.Length);
                                    if (Numerics.IsIntegral(cp.X) && Numerics.IsIntegral(cp.Y) && Numerics.IsIntegral(cp.Z))
                                    {
                                        //found
                                        twin["@ux"].Value = a;
                                        twin["@uy"].Value = b;
                                        twin["@uz"].Value = c;
                                        twin["@vx"].Value = altA;
                                        twin["@vy"].Value = altB;
                                        twin["@vz"].Value = altC;
                                        twin["@nx"].Value = (int)Math.Round(cp.X);
                                        twin["@ny"].Value = (int)Math.Round(cp.Y);
                                        twin["@nz"].Value = (int)Math.Round(cp.Z);
                                        cmdIns.ExecuteNonQuery();
                                        found = true;
                                    }
                                }
                            }
                        if(!found) throw new InvalidOperationException($"No twin vector found for {v1}");
                    }
            }
        }
    }
}
