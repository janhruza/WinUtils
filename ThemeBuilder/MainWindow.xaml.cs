using System.Windows;
using System.Windows.Media;

namespace ThemeBuilder;

/// <summary>
/// Representing the main window class.
/// </summary>
public partial class MainWindow : Window
{
    public MainWindow()
    {
        _colors = App.CreateThemeColors();
        InitializeComponent();
        LoadPages();
    }

    private void LoadPages()
    {
        if (frmGeneral != null)
        {
            frmGeneral.Content = App.pgGeneral;
        }

        if (frmColors != null)
        {
            frmColors.Content = App.pgColors;
        }

        if (frmCursors != null)
        {
            frmCursors.Content = App.pgCursors;
        }
    }

    private Dictionary<string, Color> _colors;

    private void btnClose_Click(object sender, RoutedEventArgs e)
    {
        this.Close();
    }

    private void btnCreate_Click(object sender, RoutedEventArgs e)
    {
        // create theme file
        return;
    }
}