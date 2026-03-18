// The MIT License( MIT )
//
// Copyright( c ) 2020-2026 Towel 42 Development, LLC and Scott Aron Bloom
// SPDX-License-Identifier: MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sub-license, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef _TOWEL42_MEDIAUTILS_FFMPEGUTILS_H
#define _TOWEL42_MEDIAUTILS_FFMPEGUTILS_H

#include "Towel42MediaUtilsExport.h"
#include <QStringList>
#include <QString>

#include <unordered_map>
#include <unordered_set>
#include <set>
#include <optionaL>
class QProgressDialog;
namespace NTowel42MediaUtils
{
    enum class EFormatType
    {
        eUnknown,
        eVideo,
        eAudio,
        eSubtitle,
        eImage
    };
    TOWEL42_MEDIAUTILS_EXPORT QString toString( EFormatType type );
    using TFormatMap = std::unordered_map< EFormatType, std::unordered_map< QString, QStringList > >;
    using TCodecToEncoderDecoderMap = std::unordered_map< EFormatType, std::unordered_multimap< QString, QString > >;

    class TOWEL42_MEDIAUTILS_EXPORT CFFMpegFormats
    {
    public:
        CFFMpegFormats();
        CFFMpegFormats( const QString &ffmpegExe );

        void initEncoderFormatsFromDefaults( const QStringList &terse, const QStringList &verbose, const TFormatMap &formatExtensions );
        void initDecoderFormatsFromDefaults( const QStringList &terse, const QStringList &verbose, const TFormatMap &formatExtensions );

        void initVideoEncodingCodecsFromDefaults( const QStringList &terse, const QStringList &verbose );
        void initVideoDecodingCodecsFromDefaults( const QStringList &terse, const QStringList &verbose );

        void initAudioEncodingCodecsFromDefaults( const QStringList &terse, const QStringList &verbose );
        void initAudioDecodingCodecsFromDefaults( const QStringList &terse, const QStringList &verbose );

        void initSubtitleEncodingCodecsFromDefaults( const QStringList &terse, const QStringList &verbose );
        void initSubtitleDecodingCodecsFromDefaults( const QStringList &terse, const QStringList &verbose );

        void initVideoEncodersFromDefaults( const QStringList &terse, const QStringList &verbose );
        void initVideoDecodersFromDefaults( const QStringList &terse, const QStringList &verbose );

        void initAudioEncodersFromDefaults( const QStringList &terse, const QStringList &verbose );
        void initAudioDecodersFromDefaults( const QStringList &terse, const QStringList &verbose );

        void initSubtitleEncodersFromDefaults( const QStringList &terse, const QStringList &verbose );
        void initSubtitleDecodersFromDefaults( const QStringList &terse, const QStringList &verbose );

        void initHWAccelsFromDefaults( const QStringList &terse, const QStringList &verbose );

        void initCodecToEncoderMapDefaults( const TCodecToEncoderDecoderMap &encoderMap );
        void initCodecToDecoderMapDefaults( const TCodecToEncoderDecoderMap &decoderMap );

        void setFFMpegExecutable( const QString &ffmpegExe );
        void recompute( QProgressDialog *dlg = nullptr );
        bool loaded() const { return fLoaded; }
        TFormatMap mediaEncoderFormatExtensions() const { return fMediaEncoderFormatExtensions; }
        TFormatMap mediaDecoderFormatExtensions() const { return fMediaDecoderFormatExtensions; }

        std::set< QString > getCodecAliases( const QString &formatName ) const;   // gets the tersename, the verbose name, the codecname, the encoder name and the decoder name
        bool isHEVCCodec( const QString &codec ) const { return isCodec( "hevc", codec ); }
        bool isCodec( const QString &checkCodecName, const QString &mediaCodecName ) const;

        bool isContainerFormat( const QString &fileName, const QString &formatName ) const;

        bool isEncoderFormat( const QString &suffix, const QString &formatName ) const;
        bool isDecoderFormat( const QString &suffix, const QString &formatName ) const;
        QStringList encoderFormats( bool verbose ) const;
        QStringList decoderFormats( bool verbose ) const;

        QStringList audioEncodingCodecs( bool verbose ) const;
        QStringList audioDecodingCodecs( bool verbose ) const;
        QStringList videoEncodingCodecs( bool verbose ) const;
        QStringList videoDecodingCodecs( bool verbose ) const;
        QStringList subtitleEncodingCodecs( bool verbose ) const;
        QStringList subtitleDecodingCodecs( bool verbose ) const;

        QStringList audioEncoders( bool verbose ) const;
        QStringList audioDecoders( bool verbose ) const;
        QStringList videoEncoders( bool verbose ) const;
        QStringList videoDecoders( bool verbose ) const;
        QStringList subtitleEncoders( bool verbose ) const;
        QStringList subtitleDecoders( bool verbose ) const;

        TCodecToEncoderDecoderMap codecToEncoderMap() const { return fCodecToEncoderMap; }
        TCodecToEncoderDecoderMap codecToDecoderMap() const { return fCodecToDecoderMap; }
        QStringList hwAccels( bool verbose ) const;

        QStringList getVideoExtensions( const QStringList &exclude = {} ) const;
        QStringList getSubtitleExtensions( const QStringList &exclude = {} ) const;
        QStringList getAudioExtensions( const QStringList &exclude = {} ) const;

        QStringList getVideoEncoderExtensions( const QStringList &exclude = {} ) const;
        QStringList getSubtitleEncoderExtensions( const QStringList &exclude = {} ) const;
        QStringList getAudioEncoderExtensions( const QStringList &exclude = {} ) const;

        QStringList getVideoDecoderExtensions( const QStringList &exclude = {} ) const;
        QStringList getSubtitleDecoderExtensions( const QStringList &exclude = {} ) const;
        QStringList getAudioDecoderExtensions( const QStringList &exclude = {} ) const;

        QStringList getImageEncoderExtensions( const QStringList &exclude = {} ) const;
        QStringList getImageDecoderExtensions( const QStringList &exclude = {} ) const;

        QStringList getEncoderExtensions( NTowel42MediaUtils::EFormatType extensionType, const QStringList &exclude = {} ) const;
        QStringList getDecoderExtensions( NTowel42MediaUtils::EFormatType extensionType, const QStringList &exclude = {} ) const;

        QString getPrimaryEncoderExtensionForFormat( const QString &formatName, const QStringList &exclude = {} ) const;
        QStringList getEncoderExtensionsForFormat( const QString &formatName, const QStringList &exclude = {} ) const;

        QString getPrimaryDecoderExtensionForFormat( const QString &formatName, const QStringList &exclude = {} ) const;
        QStringList getDecoderExtensionsForFormat( const QString &formatName, const QStringList &exclude = {} ) const;

        QString getTranscodeHWAccel( const QString &formatName ) const;
        QString getCodecForHWAccel( const QString &hwAccel ) const;

        static bool validateFFMpegExe( const QString &ffmpegExe );

    private:
        void postLoad();
        bool validate() const;
        void checkLoaded();

        void clear();
        bool validateFFMpegExe() const;

        void loadCodecs( QProgressDialog *dlg );

        void loadEncodersDecoders( QProgressDialog *dlg );
        void loadEncodersDecoders( bool isEncoding, QProgressDialog *dlg );

        void loadFormats( QProgressDialog *dlg );

        void loadHWAccels( QProgressDialog *dlg );

        bool isImageFormat( const QString &ext ) const;
        void computeReverseExtensionMap( bool encoders );
        void computeReverseCodecMap( bool encoders );

        void computeExtensionsForFormat( const QString &name, const QString &desc, std::optional< EFormatType > formatType, bool isEncoder );
        QStringList computeExtensionsForFormat( const QString &formatName, std::optional< EFormatType > formatType, bool isEncoder );
        std::optional< QStringList > encoderFormatLoaded( const QString &formatName ) const;
        std::optional< QStringList > decoderFormatLoaded( const QString &formatName ) const;

        QStringList getExtensionsForFormat( const TFormatMap &map, const QString &formatName, const QStringList &exclude ) const;
        QStringList getExtensions( const TFormatMap &map, NTowel42MediaUtils::EFormatType extensionType, const QStringList &exclude ) const;

        bool fLoaded{ false };
        struct SStringListPair
        {
            SStringListPair() = default;
            SStringListPair( const std::initializer_list< QStringList > &data );

            const QStringList &data( bool verbose ) const { return verbose ? fVerbose : fTerse; }
            const QStringList &terse() const { return fTerse; }
            const QStringList &verbose() const { return fVerbose; }
            void push_back( const QString &terse, const QString &verbose );

            bool isEmpty() const { return fTerse.isEmpty() || fVerbose.isEmpty(); }
            bool isLoaded() const { return !isEmpty(); }

            void clear();
            void addAlias( std::set< QString > &retVal, const QString &name ) const;
            bool validate() const;

            void sort();

        private:
            QStringList fTerse;
            QStringList fVerbose;
            std::unordered_set< QString > fExistingTerse;
        };

        struct SVideoAudioSubtitle
        {
            SVideoAudioSubtitle() = default;

            const SStringListPair &video() const { return fVideo; }
            const SStringListPair &audio() const { return fAudio; }
            const SStringListPair &subtitle() const { return fSubtitle; }

            SStringListPair &video() { return fVideo; }
            SStringListPair &audio() { return fAudio; }
            SStringListPair &subtitle() { return fSubtitle; }

            void addAlias( std::set< QString > &retVal, const QString &name ) const;
            bool isLoaded() const;
            void clear();
            bool validate() const;
            void sort();

        private:
            SStringListPair fVideo;
            SStringListPair fAudio;
            SStringListPair fSubtitle;
        };

        template< typename T >
        struct SEncoderDecoderPair
        {
            SEncoderDecoderPair() = default;

            const T &encoder() const { return fEncoder; }
            const T &decoder() const { return fDecoder; }

            T &encoder() { return fEncoder; }
            T &decoder() { return fDecoder; }

            void push_back( const T &encoder, const T &decoder )
            {
                fEncoder = encoder;
                fDecoder = decoder;
            }

            void addAlias( std::set< QString > &retVal, const QString &name ) const
            {
                fEncoder.addAlias( retVal, name );
                fDecoder.addAlias( retVal, name );
            }

            bool isLoaded() const { return fEncoder.isLoaded() && fDecoder.isLoaded(); }
            void clear()
            {
                fEncoder.clear();
                fDecoder.clear();
            }
            bool validate() const { return fEncoder.validate() && fDecoder.validate(); }
            void sort()
            {
                fEncoder.sort();
                fDecoder.sort();
            }

        private:
            T fEncoder;
            T fDecoder;
        };

        SEncoderDecoderPair< SStringListPair > fFormats;
        TFormatMap fMediaEncoderFormatExtensions;   //type -> terse format name to list of known/common extensions
        std::unordered_map< QString, QString > fExtensionToMediaEncoderFormat;   // extension to terse format
        TFormatMap fMediaDecoderFormatExtensions;   //type -> terse format name to list of known/common extensions
        std::unordered_map< QString, QString > fExtensionToMediaDecoderFormat;

        TCodecToEncoderDecoderMap fCodecToEncoderMap;   // type -> terse codec name to the list of encoders
        std::unordered_map< QString, QString > fEncoderToCodecMap;   // encoder -> codec
        TCodecToEncoderDecoderMap fCodecToDecoderMap;   // type -> terse codec name to the list of deccoders
        std::unordered_map< QString, QString > fDecoderToCodecMap;   // encoder -> codec

        SEncoderDecoderPair< SVideoAudioSubtitle > fCodecs;
        SEncoderDecoderPair< SVideoAudioSubtitle > fEncoderDecoders;

        SStringListPair fHWAccels;

        QString fFFMpegExe;
        mutable std::optional< std::unordered_set< QString > > fImageFormats;
        mutable std::unordered_map< QString, std::set< QString > > fAliases;
    };
}

#endif
