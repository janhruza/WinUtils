using System.Media;
using System.Windows;
using System.Windows.Media;

namespace ThemeBuilder.Windows;

/// <summary>
/// Representing the custom color picker window.
/// </summary>
public partial class ColorPicker : Window
{
    public ColorPicker()
    {
        InitializeComponent();
        UpdatePreview();
    }

    public ColorPicker(string colorName)
    {
        InitializeComponent();
        this.Title += $" ({colorName})";
        UpdatePreview();
    }

    public ColorPicker(string colorName, Color cValue)
    {
        InitializeComponent();
        this.Title += $" ({colorName})";
        SetInitialColor(cValue);
        UpdatePreview();
    }

    private bool bDialog;
    private Color cResult;
    public Color Result => cResult;

    public new bool? ShowDialog()
    {
        this.Loaded += (s, e) =>
        {
            SystemSounds.Beep.Play();
        };

        bDialog = true;
        return base.ShowDialog();
    }

    private new void Show()
    {
        bDialog = false;
        base.Show();
        return;
    }

    private void btnClose_Click(object sender, RoutedEventArgs e)
    {
        if (bDialog)
        {
            DialogResult = false;
        }
    }

    private void btnOk_Click(object sender, RoutedEventArgs e)
    {
        if (bDialog)
        {
            DialogResult = true;
        }

        cResult = new Color();
        cResult.A = 255;
        cResult.R = Convert.ToByte(sRed.Value);
        cResult.G = Convert.ToByte(sGreen.Value);
        cResult.B = Convert.ToByte(sBlue.Value);
    }

    private void SetInitialColor(Color color)
    {
        sRed.Value = color.R;
        sGreen.Value = color.G;
        sBlue.Value = color.B;
        return;
    }

    private void UpdatePreview()
    {
        Color c = new Color();
        c.A = 255;
        c.R = Convert.ToByte(sRed.Value);
        c.G = Convert.ToByte(sGreen.Value);
        c.B = Convert.ToByte(sBlue.Value);
        bdPreview.Background = new SolidColorBrush(c);
        return;
    }

    private void sAll_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
    {
        UpdatePreview();
    }
}
