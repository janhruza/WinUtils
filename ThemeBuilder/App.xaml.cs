using System.Windows;
using System.Windows.Media;

// custom definitions
using ColorTheme = System.Collections.Generic.Dictionary<string, System.Windows.Media.Color>;
using CursorTheme = System.Collections.Generic.Dictionary<string, string>;
using DesktopTheme = System.Collections.Generic.Dictionary<string, string>;
using VisualStylesTheme = System.Collections.Generic.Dictionary<string, string>;

namespace ThemeBuilder
{
    /// <summary>
    /// Representing the main application class.
    /// </summary>
    public partial class App : Application
    {
        private void Application_Startup(object sender, StartupEventArgs e)
        {
            MainWindow mw = new MainWindow();
            MainWindow = mw;
            mw.Show();
        }

        public static bool GenerateThemeFile(ColorTheme tColors,
                                             CursorTheme tCursors)
        {
            return true;
        }

        internal static Dictionary<string, string> AllColors => new Dictionary<string, string>
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

        internal static CursorTheme CreateThemeCursors()
        {
            return new CursorTheme
            {
                { "AppStarting", "" },
                { "Arrow", "" },
                { "Hand", "" },
                { "Help", "" },
                { "No", "" },
                { "NWPen", "" },
                { "SizeAll", "" },
                { "SizeNESW", "" },
                { "SizeNS", "" },
                { "SizeNWSE", "" },
                { "SizeWE", "" },
                { "UpArrow", "" },
                { "Wait", "" },
                { "DefaultValue", "" }
            };
        }

        internal static DesktopTheme CreateThemeDesktop()
        {
            return new DesktopTheme
            {
                { "Wallpaper", "" },
                { "Pattern", "" },
                { "MultimonBackgrounds", "" },
                { "PicturePosition", "" },
                { "WindowsSpotlight", "" }
            };
        }

        internal static VisualStylesTheme CreateVisualStylesTheme()
        {
            return new VisualStylesTheme
            {
                { "Path", "%SystemRoot%\\resources\\themes\\Aero\\Aero.msstyles" },
                { "ColorStyle", "NormalColor" },
                { "Size", "NormalSize" },
                { "AutoColorization", "0" },
                { "ColorizationColor", "0xFF357EC7" }, // Windows Blue
                { "SystemMode", "Light" },
                { "AppMode", "Light" },
                { "VisualStyleVersion", "10" },
            };
        }
    }

}
