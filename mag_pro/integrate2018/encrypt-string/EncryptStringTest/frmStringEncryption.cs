using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EncryptStringTest
{
    public partial class frmStringEncryption : Form
    {
        public frmStringEncryption()
        {
            InitializeComponent();
        }

        private void butEncrypt_Click(object sender, EventArgs e)
        {
            if ((!string.IsNullOrWhiteSpace(textBoxString.Text) && !string.IsNullOrWhiteSpace(textBoxString.Text))==true)
            {
                try
                {
                    textBoxEncrypted.Text = Encrypt.EncryptString(textBoxString.Text, textBoxPassword.Text);
                    textBoxString.Text = "";
                }
                catch( Exception ex )
                {
                    MessageBox.Show(ex.ToString());
                }
            }
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
        private void getdatafromtextbox(byte[] hexval)
        {
            int len = 0, lenhex = 0;
            string hexout;

            //string text = textBoxString.Text;
            // remove all space char in textbox
            textBoxString.Text = textBoxString.Text.Replace(" ", "");
            string text = textBoxString.Text;
            char[] chars = text.ToCharArray();
            len = text.Length;
            //var hexval = new byte[len / 2];
            for (int i = 0; i < text.Length; i++)
            {
                hexout = chars[i].ToString() + chars[i + 1].ToString();
                i++;
                if (hexout != "\r\n") hexval[lenhex++] = Convert.ToByte(hexout, 16);
            }
        }
        private void butDecrypt_Click(object sender, EventArgs e)
        {
            //byte[] datatest= { 0x67, 0x43, 0xC3, 0xD1, 0x51, 0x9A, 0xB4, 0xF2, 0xCD, 0x9A, 0x78, 0xAB, 0x09, 0xA5, 0x11, 0xBD };
            char[] allowedChars = new char[] { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
            var keyhex = new byte[16] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; 
            var datahex= new byte[16] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
            int len = 0, lenhex = 0;
            string hexout;
            // remove all space char in textbox
            textBoxString.Text = textBoxString.Text.Replace(" ", "");
            string text = textBoxString.Text;
            char[] chars = text.ToCharArray();
            len = text.Length;
            var hexval = new byte[len / 2];

            if (textBoxString.Text == "") return;

            for (int i = 0; i < text.Length; i++)
            {
                hexout = chars[i].ToString() + chars[i + 1].ToString();
                i++;
                if (hexout != "\r\n") hexval[lenhex++] = Convert.ToByte(hexout, 16);
            }

            getkey(keyhex);

            

            //textBoxPassword.Text = string.Join(" ", keytest.Select(x => x.ToString("X2")));
            if (textBoxString.Text!="")
            {
                string datainput = textBoxString.Text;
                string[] hexValuesSplit = datainput.Split(' ');
                int i = 0;
                foreach (String hex in hexValuesSplit)
                {
                    foreach (char character in hex.ToUpper().ToArray())
                    {
                        if (!allowedChars.Contains(character))
                        {
                            System.Windows.Forms.MessageBox.Show(string.Format("'{0}' is not a hexadecimal character", character));
                            textBoxString.Text = "";
                            return;
                        }
                    }
               
                    if (hex.Length == 2)
                    {
                        // Convert the number expressed in base-16 to an integer.
                        byte value = Convert.ToByte(hex, 16);
                        datahex[i++] = value;
                    }
                    else
                    {
                        System.Windows.Forms.MessageBox.Show(string.Format("Wrong format"));
                    }
                    
                }
            }
            Encrypt.aes_enc_dec(datahex, keyhex, 1);
            //textBoxEncrypted.Text = string.Join(" ", datahex.Select(x => x.ToString("X2")));
            for (int j = 0; j < 16; j++)
            {
                if (datahex[j] == 0) return;
                textBoxEncrypted.Text = textBoxEncrypted.Text + (char) datahex[j];     
            }
            
            // byte[] datahex = Convert.ToByte(datainput);
            //textBoxString.Text = string.Join(" ", keyhex.Select(x => x.ToString("X2")));

            // textBoxEncrypted.Text = string.Join(" ", datahex.Select(x => x.ToString("X2")));          
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int len=0,lenhex=0;
            string hexout;
           
            //string text = textBoxString.Text;
            // remove all space char in textbox
            textBoxString.Text = textBoxString.Text.Replace(" ", "");
            string text = textBoxString.Text;
            char[] chars = text.ToCharArray();
            len = text.Length;
            var hexval = new byte[len/2];
            for (int i = 0; i < text.Length; i++)
            {
                hexout = chars[i].ToString() + chars[i + 1].ToString();
                i++;
                if(hexout!="\r\n") hexval[lenhex++] = Convert.ToByte(hexout, 16);
            }
                      
        }
    }
}
