using System.Text;
using System.Windows;
using System.Windows.Controls;

namespace ThemeBuilder.Pages;

/// <summary>
/// Representing the cursors page.
/// </summary>
public partial class PgCursors : Page, IThemeBuilder
{
    public string ThemeSection => "[Control Panel\\Cursors]";

    public string BuildThemeSection()
    {
        StringBuilder sb = new StringBuilder();
        sb.AppendLine(ThemeSection);

        foreach (var kp in dCursors)
        {
            sb.AppendLine($"{kp.Key}={GetValue(kp.Key)}");
        }

        return sb.ToString();
    }

    public PgCursors()
    {
        InitializeComponent();
        dCursors = App.CreateThemeCursors();
        lWrappers = new List<InputWrapper>();

        for (int x = 0; x < dCursors.Count; x++)
        {
            lWrappers.Add(new InputWrapper());
        }

        RefreshUI();
    }

    private Dictionary<string, string> dCursors;
    private List<InputWrapper> lWrappers;

    private string GetValue(string sKey)
    {
        int index = dCursors.Keys.ToList().IndexOf(sKey);
        return lWrappers[index].Value;
    }

    private void RefreshUI()
    {
        if (dCursors == null) return;

        stp.Children.Clear();

        for (int x = 0; x < dCursors.Count; x++)
        {
            var kp = dCursors.ElementAt(x);
            Border bd = App.CreateOpenFileItem(kp.Key, lWrappers[x], "Cursors|*.cur;*.ani|Other|*.*", new System.Windows.Thickness(0, (x > 0 ? 5 : 0), 0, 0));
            stp.Children.Add(bd);
        }
    }
}
