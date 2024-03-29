/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#if JUCE_USE_DIRECTWRITE
namespace
{
    static String getLocalisedName (IDWriteLocalizedStrings* names)
    {
        jassert (names != nullptr);

        uint32 index = 0;
        BOOL exists = false;
        HRESULT hr = names->FindLocaleName (L"en-us", &index, &exists);
        if (! exists)
            index = 0;

        uint32 length = 0;
        hr = names->GetStringLength (index, &length);

        HeapBlock<wchar_t> name (length + 1);
        hr = names->GetString (index, name, length + 1);

        return static_cast <const wchar_t*> (name);
    }

    static String getFontFamilyName (IDWriteFontFamily* family)
    {
        jassert (family != nullptr);
        ComSmartPtr<IDWriteLocalizedStrings> familyNames;
        HRESULT hr = family->GetFamilyNames (familyNames.resetAndGetPointerAddress());
        jassert (SUCCEEDED (hr)); (void) hr;
        return getLocalisedName (familyNames);
    }

    static String getFontFaceName (IDWriteFont* font)
    {
        jassert (font != nullptr);
        ComSmartPtr<IDWriteLocalizedStrings> faceNames;
        HRESULT hr = font->GetFaceNames (faceNames.resetAndGetPointerAddress());
        jassert (SUCCEEDED (hr)); (void) hr;

        return getLocalisedName (faceNames);
    }
}

class Direct2DFactories
{
public:
    Direct2DFactories()
    {
        if (direct2dDll.open ("d2d1.dll"))
        {
            JUCE_DLL_FUNCTION (D2D1CreateFactory, d2d1CreateFactory, HRESULT, direct2dDll, (D2D1_FACTORY_TYPE, REFIID, D2D1_FACTORY_OPTIONS*, void**))

            if (d2d1CreateFactory != nullptr)
            {
                D2D1_FACTORY_OPTIONS options;
                options.debugLevel = D2D1_DEBUG_LEVEL_NONE;

                d2d1CreateFactory (D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof (ID2D1Factory), &options,
                                   (void**) d2dFactory.resetAndGetPointerAddress());
            }
        }

        if (directWriteDll.open ("DWrite.dll"))
        {
            JUCE_DLL_FUNCTION (DWriteCreateFactory, dWriteCreateFactory, HRESULT, directWriteDll, (DWRITE_FACTORY_TYPE, REFIID, IUnknown**))

            if (dWriteCreateFactory != nullptr)
            {
                dWriteCreateFactory (DWRITE_FACTORY_TYPE_SHARED, __uuidof (IDWriteFactory),
                                     (IUnknown**) directWriteFactory.resetAndGetPointerAddress());

                if (directWriteFactory != nullptr)
                    directWriteFactory->GetSystemFontCollection (systemFonts.resetAndGetPointerAddress());
            }
        }
    }

    ~Direct2DFactories()
    {
        d2dFactory = nullptr;  // (need to make sure these are released before deleting the DynamicLibrary objects)
        directWriteFactory = nullptr;
        systemFonts = nullptr;
    }

    static const Direct2DFactories& getInstance()
    {
        static Direct2DFactories instance;
        return instance;
    }

    ComSmartPtr <ID2D1Factory> d2dFactory;
    ComSmartPtr <IDWriteFactory> directWriteFactory;
    ComSmartPtr <IDWriteFontCollection> systemFonts;

private:
    DynamicLibrary direct2dDll, directWriteDll;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Direct2DFactories);
};

//==================================================================================================
class WindowsDirectWriteTypeface  : public Typeface
{
public:
    WindowsDirectWriteTypeface (const Font& font, IDWriteFontCollection* fontCollection)
        : Typeface (font.getTypefaceName(), font.getTypefaceStyle()),
          ascent (0.0f)
    {
        jassert (fontCollection != nullptr);

        BOOL fontFound = false;
        uint32 fontIndex = 0;
        HRESULT hr = fontCollection->FindFamilyName (font.getTypefaceName().toWideCharPointer(), &fontIndex, &fontFound);
        if (! fontFound)
            fontIndex = 0;

        // Get the font family using the search results
        // Fonts like: Times New Roman, Times New Roman Bold, Times New Roman Italic are all in the same font family
        ComSmartPtr<IDWriteFontFamily> dwFontFamily;
        hr = fontCollection->GetFontFamily (fontIndex, dwFontFamily.resetAndGetPointerAddress());

        // Get a specific font in the font family using typeface style
        ComSmartPtr<IDWriteFont> dwFont;
        uint32 fontFacesCount = 0;
        fontFacesCount = dwFontFamily->GetFontCount();

        bool styleFound = false;
        for (uint32 i = 0; i < fontFacesCount; ++i)
        {
            hr = dwFontFamily->GetFont (i, dwFont.resetAndGetPointerAddress());

            ComSmartPtr<IDWriteLocalizedStrings> faceNames;
            hr = dwFont->GetFaceNames (faceNames.resetAndGetPointerAddress());
            if (font.getTypefaceStyle() == getLocalisedName (faceNames))
            {
                styleFound = true;
                break;
            }
        }
        if (!styleFound)
        {
            hr = dwFontFamily->GetFont (0, dwFont.resetAndGetPointerAddress());
            ComSmartPtr<IDWriteLocalizedStrings> faceNames;
            hr = dwFont->GetFaceNames (faceNames.resetAndGetPointerAddress());
        }

        hr = dwFont->CreateFontFace (dwFontFace.resetAndGetPointerAddress());

        DWRITE_FONT_METRICS dwFontMetrics;
        dwFontFace->GetMetrics (&dwFontMetrics);

        // All Font Metrics are in design units so we need to get designUnitsPerEm value to get the metrics
        // into Em/Design Independent Pixels
        designUnitsPerEm = dwFontMetrics.designUnitsPerEm;

        ascent = std::abs ((float) dwFontMetrics.ascent);
        const float totalSize = ascent + std::abs ((float) dwFontMetrics.descent);
        ascent /= totalSize;
        unitsToHeightScaleFactor = 1.0f / (totalSize / designUnitsPerEm);
        const float pathAscent = (((float) dwFontMetrics.ascent) / ((float) designUnitsPerEm)) * 1024.0f;
        const float pathDescent = (((float) dwFontMetrics.descent) / ((float) designUnitsPerEm)) * 1024.0f;
        const float pathTotalSize = std::abs (pathAscent) + std::abs (pathDescent);
        pathTransform = AffineTransform::identity.scale (1.0f / pathTotalSize, 1.0f / pathTotalSize);
    }

    float getAscent() const     { return ascent; }
    float getDescent() const    { return 1.0f - ascent; }

    float getStringWidth (const String& text)
    {
        const CharPointer_UTF32 textUTF32 (text.toUTF32());
        const size_t len = textUTF32.length();

        HeapBlock <UINT16> glyphIndices (len);
        dwFontFace->GetGlyphIndices (textUTF32, (UINT32) len, glyphIndices);

        HeapBlock <DWRITE_GLYPH_METRICS> dwGlyphMetrics (len);
        dwFontFace->GetDesignGlyphMetrics (glyphIndices, (UINT32) len, dwGlyphMetrics, false);

        float x = 0;
        for (size_t i = 0; i < len; ++i)
            x += (float) dwGlyphMetrics[i].advanceWidth / designUnitsPerEm;

        return x * unitsToHeightScaleFactor;
    }

    void getGlyphPositions (const String& text, Array <int>& resultGlyphs, Array <float>& xOffsets)
    {
        xOffsets.add (0);

        const CharPointer_UTF32 textUTF32 (text.toUTF32());
        const size_t len = textUTF32.length();

        HeapBlock <UINT16> glyphIndices (len);
        dwFontFace->GetGlyphIndices (textUTF32, (UINT32) len, glyphIndices);
        HeapBlock <DWRITE_GLYPH_METRICS> dwGlyphMetrics (len);
        dwFontFace->GetDesignGlyphMetrics (glyphIndices, (UINT32) len, dwGlyphMetrics, false);

        float x = 0;
        for (size_t i = 0; i < len; ++i)
        {
            x += (float) dwGlyphMetrics[i].advanceWidth / designUnitsPerEm;
            xOffsets.add (x * unitsToHeightScaleFactor);
            resultGlyphs.add (glyphIndices[i]);
        }
    }

    EdgeTable* getEdgeTableForGlyph (int glyphNumber, const AffineTransform& transform)
    {
        Path path;

        if (getOutlineForGlyph (glyphNumber, path) && ! path.isEmpty())
            return new EdgeTable (path.getBoundsTransformed (transform).getSmallestIntegerContainer().expanded (1, 0),
                                  path, transform);

        return nullptr;
    }

    bool getOutlineForGlyph (int glyphNumber, Path& path)
    {
        jassert (path.isEmpty());  // we might need to apply a transform to the path, so this must be empty
        UINT16 glyphIndex = (UINT16) glyphNumber;
        ComSmartPtr<PathGeometrySink> pathGeometrySink (new PathGeometrySink());

        dwFontFace->GetGlyphRunOutline (1024.0f, &glyphIndex, nullptr, nullptr, 1, false, false, pathGeometrySink);
        path = pathGeometrySink->path;

        if (! pathTransform.isIdentity())
            path.applyTransform (pathTransform);

        return true;
    }

private:
    ComSmartPtr<IDWriteFontFace> dwFontFace;
    float unitsToHeightScaleFactor, ascent;
    int designUnitsPerEm;
    AffineTransform pathTransform;

    class PathGeometrySink  : public ComBaseClassHelper<IDWriteGeometrySink>
    {
    public:
        PathGeometrySink()   { resetReferenceCount(); }

        void __stdcall AddBeziers (const D2D1_BEZIER_SEGMENT *beziers, UINT beziersCount)
        {
            for (UINT i = 0; i < beziersCount; ++i)
                path.cubicTo ((float) beziers[i].point1.x, (float) beziers[i].point1.y,
                              (float) beziers[i].point2.x, (float) beziers[i].point2.y,
                              (float) beziers[i].point3.x, (float) beziers[i].point3.y);
        }

        void __stdcall AddLines (const D2D1_POINT_2F* points, UINT pointsCount)
        {
            for (UINT i = 0; i < pointsCount; ++i)
                path.lineTo ((float) points[i].x,
                             (float) points[i].y);
        }

        void __stdcall BeginFigure (D2D1_POINT_2F startPoint, D2D1_FIGURE_BEGIN)
        {
            path.startNewSubPath ((float) startPoint.x,
                                  (float) startPoint.y);
        }

        void __stdcall EndFigure (D2D1_FIGURE_END figureEnd)
        {
            if (figureEnd == D2D1_FIGURE_END_CLOSED)
                path.closeSubPath();
        }

        void __stdcall SetFillMode (D2D1_FILL_MODE fillMode)
        {
            path.setUsingNonZeroWinding (fillMode == D2D1_FILL_MODE_WINDING);
        }

        void __stdcall SetSegmentFlags (D2D1_PATH_SEGMENT) {}
        JUCE_COMRESULT Close()  { return S_OK; }

        Path path;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PathGeometrySink);
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WindowsDirectWriteTypeface);
};

#endif
