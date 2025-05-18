using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace ThemeBuilder.Pages;

/// <summary>
/// Representing the colors page.
/// </summary>
public partial class PgColors : Page, IThemeBuilder
{
    public PgColors()
    {
        InitializeComponent();
        dColors = App.CreateThemeColors();
        RefreshUI();
    }

    public string ThemeSection => "[Control Panel\\Colors]";

    public string BuildThemeSection()
    {
        StringBuilder sb = new StringBuilder();
        sb.AppendLine(ThemeSection);

        foreach (var pair in dColors)
        {
            string sValue = $"{pair.Value.R} {pair.Value.G} {pair.Value.B}";
            sb.AppendLine($"{pair.Key}={sValue}");
        }

        return sb.ToString();
    }

    private Dictionary<string, Color> dColors;

    private void RefreshUI()
    {
        if (dColors == null) return;

        stp.Children.Clear();
        foreach (var kp in dColors)
        {
            Border bd = App.CreateColorPickerItem(dColors, kp.Key);
            stp.Children.Add(bd);
        }

        return;
    }
}
