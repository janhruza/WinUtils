using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using ThemeBuilder.Windows;

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
        ReloadColors();
    }

    private Dictionary<string, Color> _colors;

    private Border CreateColorItem(KeyValuePair<string, string> colorDictItem)
    {
        Border bd = new Border();

        Grid g = new Grid();
        g.ColumnDefinitions.Add(new ColumnDefinition());
        g.ColumnDefinitions.Add(new ColumnDefinition() { Width = GridLength.Auto});

        Label lbl = new Label();
        lbl.Content = colorDictItem.Value;
        lbl.VerticalAlignment = VerticalAlignment.Center;

        Button btn = new Button();
        btn.Content = "...";

        btn.Click += (s, e) =>
        {
            // open color dialog
            Color c = ((SolidColorBrush)btn.Background).Color;
            ColorPicker cp = new ColorPicker(colorDictItem.Key, c);

            if (cp.ShowDialog() == true)
            {
                // set color selected by user
                _colors[colorDictItem.Key] = cp.Result;
                btn.Background = new SolidColorBrush(cp.Result);
            }
        };

        g.Children.Add(lbl);
        g.Children.Add(btn);

        Grid.SetColumn(btn, 1);
        bd.Child = g;
        return bd;
    }

    private bool ReloadColors()
    {
        stpColors.Children.Clear();

        foreach (KeyValuePair<string, string> entry in App.AllColors)
        {
            Border bd = CreateColorItem(entry);
            stpColors.Children.Add(bd);
        }

        return true;
    }
}