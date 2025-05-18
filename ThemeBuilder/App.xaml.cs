using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using Microsoft.Win32;
using ThemeBuilder.Pages;
using ThemeBuilder.Windows;

// custom definitions
using ColorTheme = System.Collections.Generic.Dictionary<string, System.Windows.Media.Color>;   // Color theme data
using ThemeDict = System.Collections.Generic.Dictionary<string, string>;                        // Generic theme dictionary

namespace ThemeBuilder
{
    /// <summary>
    /// Representing the main application class.
    /// </summary>
    public partial class App : Application
    {
        // page declarations
        internal static PgGeneral pgGeneral;
        internal static PgColors pgColors;
        internal static PgCursors pgCursors;
        private static bool LoadPages()
        {
            try
            {
                pgGeneral = new PgGeneral();
                pgColors = new PgColors();
                pgCursors = new PgCursors();
                return true;
            }

            catch (Exception ex)
            {
                _ = MessageBox.Show(ex.Message, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return false;
            }
        }

        private void Application_Startup(object sender, StartupEventArgs e)
        {
            // load pages
            if (LoadPages() == false)
            {
                Shutdown(-1);
                return;
            }

            MainWindow mw = new MainWindow();
            MainWindow = mw;
            mw.Show();
            return;
        }

        /// <summary>
        /// Generates a theme file based on the provided theme components.
        /// </summary>
        /// <param name="tColors">Color data.</param>
        /// <param name="tCursors">Cursor data.</param>
        /// <param name="tDesktop">Desktop data.</param>
        /// <param name="tVisualStyles">Visual styles data.</param>
        /// <returns><see langword="true"/> if theme is generated, otherwise <see langword="false"/>.</returns>
        public static bool GenerateThemeFile(ColorTheme tColors,
                                             ThemeDict tCursors,
                                             ThemeDict tDesktop,
                                             ThemeDict tVisualStyles)
        {
            // add master theme sector
            // add theme colors
            // add theme cursors
            // add theme desktop settings
            // add theme visual styles settings
            // add other theme data
            return true;
        }

        internal static ThemeDict AllColors => new ThemeDict
        {
            { "ScrollBar", "Scroll bar" },
            { "Background", "Background" },
            { "ActiveTitle", "Active title" },
            { "InactiveTitle", "Inactive title" },
            { "Menu", "Menu" },
            { "Window", "Window background" },
            { "WindowFrame", "Window frame" },
            { "MenuText", "Menu text" },
            { "WindowText", "Window text" },
            { "TitleText", "Title text" },
            { "ActiveBorder", "Active border" },
            { "InactiveBorder", "Inactive border" },
            { "AppWorkspace", "Application workspace" },
            { "Hilight", "Highlight" },
            { "HilightText", "Hilight text" },
            { "ButtonFace", "Button face" },
            { "ButtonShadow", "Button shadow" },
            { "GrayText", "Gray text" },
            { "ButtonText", "Button text" },
            { "InactiveTitleText", "Inactive title text" },
            { "ButtonHilight", "Button highlight" },
            { "ButtonDkShadow", "Button DK shadow" },
            { "ButtonLight", "Button light" },
            { "InfoText", "Info text" },
            { "InfoWindow", "Info window" },
            { "ButtonAlternativeFace", "Button alternative face" },
            { "HotTrackingColor", "Hot tracking color" },
            { "GradientActiveTitle", "Gradient active title" },
            { "GradientInactiveTitle", "Gradient inactive title" },
            { "MenuHilight", "Menu highlight" },
            { "MenuBar", "Menu bar" }
        };

        internal static ColorTheme CreateThemeColors()
        {
            return new ColorTheme
            {
                { "ScrollBar", new Color() },
                { "Background", new Color() },
                { "ActiveTitle", new Color() },
                { "InactiveTitle", new Color() },
                { "Menu", new Color() },
                { "Window", new Color() },
                { "WindowFrame", new Color() },
                { "MenuText", new Color() },
                { "WindowText", new Color() },
                { "TitleText", new Color() },
                { "ActiveBorder", new Color() },
                { "InactiveBorder", new Color() },
                { "AppWorkspace", new Color() },
                { "Hilight", new Color() },
                { "HilightText", new Color() },
                { "ButtonFace", new Color() },
                { "ButtonShadow", new Color() },
                { "GrayText", new Color() },
                { "ButtonText", new Color() },
                { "InactiveTitleText", new Color() },
                { "ButtonHilight", new Color() },
                { "ButtonDkShadow", new Color() },
                { "ButtonLight", new Color() },
                { "InfoText", new Color() },
                { "InfoWindow", new Color() },
                { "ButtonAlternativeFace", new Color() },
                { "HotTrackingColor", new Color() },
                { "GradientActiveTitle", new Color() },
                { "GradientInactiveTitle", new Color() },
                { "MenuHilight", new Color() },
                { "MenuBar", new Color() }
            };
        }

        internal static ThemeDict CreateThemeCursors()
        {
            return new ThemeDict
            {
                { "AppStarting", "" },
                { "Arrow", "" },
                { "Hand", "" },
                { "Help", "" },
                { "No", "" },
                { "NWPen", "" },
                { "SizeAll", "" },
                { "SizeNESW", "" },
                { "IBeam", "" },
                { "Crosshair", "" },
                { "SizeNS", "" },
                { "SizeNWSE", "" },
                { "SizeWE", "" },
                { "UpArrow", "" },
                { "Wait", "" },
                { "DefaultValue", "Windows default" }
            };
        }

        internal static ThemeDict CreateThemeDesktop()
        {
            return new ThemeDict
            {
                { "Wallpaper", "" },
                { "Pattern", "" },
                { "MultimonBackgrounds", "" },
                { "PicturePosition", "" },
                { "WindowsSpotlight", "" }
            };
        }

        internal static ThemeDict CreateVisualStylesTheme()
        {
            return new ThemeDict
            {
                { "Path", "%SystemRoot%\\resources\\themes\\Aero\\Aero.msstyles" },
                { "ColorStyle", "NormalColor" },
                { "Size", "NormalSize" },
                { "AutoColorization", "0" },
                { "ColorizationColor", "0xFF357EC7" }, // Windows Blue
                { "SystemMode", "Light" },
                { "AppMode", "Light" },
                { "VisualStyleVersion", "10" }
            };
        }

        internal static Border CreateInputControl(string label, InputWrapper data, Thickness margin = default)
        {
            Border bd = new Border
            {
                Margin = margin
            };

            StackPanel sp = new StackPanel();
            Label lbl = new Label
            {
                Content = label
            };

            TextBox txt = new TextBox
            {
                SelectionBrush = SystemColors.AccentColorBrush
            };

            txt.TextChanged += (s, e) =>
            {
                data.Value = txt.Text;
            };

            sp.Children.Add(lbl);
            sp.Children.Add(txt);
            bd.Child = sp;
            return bd;
        }

        internal static Border CreateColorPickerItem(ColorTheme tColors, string sKey)
        {
            Border bd = new Border();

            Grid g = new Grid();
            g.ColumnDefinitions.Add(new ColumnDefinition());
            g.ColumnDefinitions.Add(new ColumnDefinition() { Width = GridLength.Auto});

            Label lbl = new Label
            {
                Content = App.AllColors[sKey],
                VerticalAlignment = VerticalAlignment.Center
            };

            Button btn = new Button
            {
                Content = "..."
            };

            btn.Click += (s, e) =>
            {
                ColorPicker picker = new ColorPicker(sKey, tColors[sKey]);
                if (picker.ShowDialog() == true)
                {
                    tColors[sKey] = picker.Result;
                    btn.Background = new SolidColorBrush(picker.Result);
                }
            };

            g.Children.Add(lbl);
            g.Children.Add(btn);
            Grid.SetColumn(btn, 1);

            bd.Child = g;
            return bd;
        }

        internal static Border CreateOpenFileItem(string label, InputWrapper data, string sFilter = "Other|*.*", Thickness margin = default)
        {
            Border bd = new Border
            {
                Margin = margin
            };

            StackPanel sp = new StackPanel();
            Label lbl = new Label
            {
                Content = label
            };
            sp.Children.Add(lbl);

            Grid g = new Grid();
            g.ColumnDefinitions.Add(new ColumnDefinition());
            g.ColumnDefinitions.Add(new ColumnDefinition() { Width = GridLength.Auto });

            TextBox txt = new TextBox
            {
                SelectionBrush = SystemColors.AccentColorBrush
            };

            txt.TextChanged += (s, e) => data.Value = txt.Text;
            g.Children.Add(txt);

            Button btnOpen = new Button
            {
                Content = "...",
                Margin = new Thickness(5, 0, 0, 0)
            };

            btnOpen.Click += (s, e) =>
            {
                OpenFileDialog ofd = new OpenFileDialog
                {
                    Filter = sFilter
                };

                if (ofd.ShowDialog() == true)
                {
                    txt.Text = ofd.FileName;
                }
            };

            g.Children.Add(btnOpen);
            Grid.SetColumn(btnOpen, 1);

            sp.Children.Add(g);
            bd.Child = sp;
            return bd;
        }
    }

}
