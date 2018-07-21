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

        private void butDecrypt_Click(object sender, EventArgs e)
        {
            byte[] datatest= { 0x67, 0x43, 0xC3, 0xD1, 0x51, 0x9A, 0xB4, 0xF2, 0xCD, 0x9A, 0x78, 0xAB, 0x09, 0xA5, 0x11, 0xBD };
            byte[] keytest = { 0xE8, 0xE9, 0xEA, 0xEB, 0xED, 0xEE, 0xEF, 0xF0, 0xF2, 0xF3, 0xF4, 0xF5, 0xF7, 0xF8, 0xF9, 0xFA };
            textBoxPassword.Text = string.Join(" ", keytest.Select(x => x.ToString("X2")));
            textBoxString.Text = string.Join(" ", datatest.Select(x => x.ToString("X2")));
            Encrypt.aes_enc_dec(datatest, keytest, 1);
            textBoxEncrypted.Text = string.Join(" ", datatest.Select(x => x.ToString("X2")));          
        }
    }
}
