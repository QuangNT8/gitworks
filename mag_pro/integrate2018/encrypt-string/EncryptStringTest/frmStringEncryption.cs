using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EncryptStringTest
{
    public partial class frmStringEncryption : Form
    {
        string textinput;
        string textoutput;

        public frmStringEncryption()
        {
            InitializeComponent();
        }

        
        private void getkey(byte[] keyhex)
        {
            byte[] buffer = ASCIIEncoding.ASCII.GetBytes(textBoxPassword.Text);
            buffer = ASCIIEncoding.ASCII.GetBytes(textBoxPassword.Text);
            for (int j = 0; j < keyhex.Length; j++)
            {
                if (j < buffer.Length) keyhex[j] = buffer[j];
            }
        }

        private void butDecrypt_Click(object sender, EventArgs e)
        {
            butDecrypt.Enabled = false;
            //byte[] datatest= { 0x67, 0x43, 0xC3, 0xD1, 0x51, 0x9A, 0xB4, 0xF2, 0xCD, 0x9A, 0x78, 0xAB, 0x09, 0xA5, 0x11, 0xBD };
            char[] allowedChars = new char[] { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
            var keyhex = new byte[16] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
            var datahex = new byte[16] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
            int len = 0, lenhex = 0;
            byte[] tempkey;
            string hexout;
            // remove all space char in textbox
            editdata();
            //textBoxEncrypted.Text = "";
            textoutput = "";

            string text = textinput;
            char[] chars = text.ToCharArray();
            len = text.Length;
            var hexval = new byte[len / 2];

            if (textinput == "")
            {
                butDecrypt.Enabled = true;
                return;
            }

            for (long i = 0; i < text.Length; i++)
            {
                hexout = chars[i].ToString() + chars[i + 1].ToString();
                i++;
                if ((hexout != "\r\n") && (hexout != "\n\r")) hexval[lenhex++] = Convert.ToByte(hexout, 16);
            }

            getkey(keyhex);

            long v,j,u,buflen;
            buflen = hexval.Length/16;
            for (v = 0; v < buflen; v++)
            {
                for (j = 0; j < 16; j++)
                {
                    datahex[j] = hexval[j+(v*16)];
                }
                tempkey = keyhex;
                Encrypt.aes_enc_dec(datahex, tempkey, 1);
                //textBoxEncrypted.Text = string.Join(" ", datahex.Select(x => x.ToString("X2")));
                for (u = 0; u < 16; u++)
                {
                    //if (datahex[j] == 0) return;
                    if (datahex[u] == 255)
                    {
                        //textoutput = textBoxEncrypted.Text + "\r\nTrack1: ";
                        textoutput = textoutput + "\r\nTrack1: ";
                    }
                    else if (datahex[u] == 254)
                    {
                        //textBoxEncrypted.Text = textBoxEncrypted.Text + "\r\nTrack2: ";
                        textoutput = textoutput + "\r\nTrack2: ";
                    }
                    else if (datahex[u] == 252)
                    {
                        //textBoxEncrypted.Text = textBoxEncrypted.Text + "\r\nPin: ";
                        textoutput = textoutput + "\r\nPin: ";
                    }
                    else
                    {
                        //textBoxEncrypted.Text = textBoxEncrypted.Text + (char)datahex[u];
                        textoutput= textoutput + (char)datahex[u];
                    }
                    
                    //if (datahex[u] == '?') textBoxEncrypted.Text = textBoxEncrypted.Text + "\r\n";
                }
            }

            savefiles();
            // byte[] datahex = Convert.ToByte(datainput);
            //textBoxString.Text = string.Join(" ", keyhex.Select(x => x.ToString("X2")));
            butDecrypt.Enabled = true;
            // textBoxEncrypted.Text = string.Join(" ", datahex.Select(x => x.ToString("X2")));          
        }

        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "Text|*.txt|All File()|*.*";
            ofd.Title = "Open File...";
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                //ReadFile(ofd.FileName);
                textinput = File.ReadAllText(ofd.FileName);
                label1.Text = ofd.FileName;
            }
        }
        // String Collection Method.
        private void ReadFile(string file)
        {
            StreamReader reader = new StreamReader(file, Encoding.UTF8);
            //textBoxString.Text = reader.ReadToEnd();
            textinput = reader.ReadToEnd();
            reader.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "Text|*.txt";
            sfd.Title = "Save file...";
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                WriteFile(sfd.FileName);
            }
        }
        private void savefiles()
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "Text|*.txt";
            sfd.Title = "Save file...";
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                File.WriteAllText(sfd.FileName,textoutput);
                label2.Text = sfd.FileName;
                //WriteFile(sfd.FileName);
            }
        }
        void WriteFile(string file)
        {
            /*
            StreamWriter sw = new StreamWriter(file, false,
                                  Encoding.Unicode);
            foreach (string line in textBoxEncrypted.Lines)
            {
                sw.WriteLine(line);
            }
            sw.Close();*/
        }

        private void editdata()
        {
            /*textBoxString.Text = textBoxString.Text.Replace("password:", "");
            textBoxString.Text = textBoxString.Text.Replace("OK", "");
            textBoxString.Text = textBoxString.Text.Replace(">d", "");
            textBoxString.Text = textBoxString.Text.Replace(">e\n\r exit", "");
            textBoxString.Text = textBoxString.Text.Replace(">c\n\r[M][3]01/01 00:00:00", "");
            textBoxString.Text = textBoxString.Text.Replace("exit", "");
            textBoxString.Text = textBoxString.Text.Replace(" ", "");
            textBoxString.Text = textBoxString.Text.Replace("X", "");
            textBoxString.Text = textBoxString.Text.Replace("*", "");
            textBoxString.Text = textBoxString.Text.Replace(">", "");
            textBoxString.Text = textBoxString.Text.Replace("<", "");
            textBoxString.Text = textBoxString.Text.Replace("\n", "");
            textBoxString.Text = textBoxString.Text.Replace("\r", "");*/
            textinput = textinput.Replace("password:", "");
            textinput = textinput.Replace("OK", "");
            textinput = textinput.Replace(">d", "");
            textinput = textinput.Replace(">e\n\r exit", "");
            textinput = textinput.Replace(">c\n\r[M][3]01/01 00:00:00", "");
            textinput = textinput.Replace("exit", "");
            textinput = textinput.Replace(" ", "");
            textinput = textinput.Replace("X", "");
            textinput = textinput.Replace("*", "");
            textinput = textinput.Replace(">", "");
            textinput = textinput.Replace("<", "");
            textinput = textinput.Replace("\n", "");
            textinput = textinput.Replace("\r", "");
        }
    }
}
