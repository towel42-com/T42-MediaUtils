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
// AUTHORS OR COPYRIGHT HOLdingCodecs BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "FFMpegFormats.h"
#include "T42-Utils/GPUDetect.h"

#include <QProcess>
#include <QFileInfo>
#include <QProgressDialog>
#include <QRegularExpression>
#include <QImageReader>

#include <set>

namespace NTowel42MediaUtils
{
    QStringList removeFromlist( const QStringList &list, const QString &regex )
    {
        auto retVal = std::move( list );

        auto regExp = QRegularExpression( regex );
        auto pos = retVal.indexOf( regExp );
        while ( pos != -1 )
        {
            retVal.removeAt( pos );
            pos = retVal.indexOf( regExp, pos );
        }

        return std::move( retVal );
    }

    QStringList removeFromlist( const QStringList &list, const QStringList &exclude )
    {
        auto retVal = std::move( list );

        for ( auto &&ii : exclude )
            retVal.removeAll( ii );

        return std::move( retVal );
    }

    void cleanLists( const QString &intel, const QString &nvidia, const QString &amd, QStringList &terse, QStringList &verbose )
    {
        NTowel42Utils::SGPUInfo info;
        QStringList regExs;
        if ( !info.fIntel )
        {
            regExs << intel;
        }

        if ( !info.fNVidia )
        {
            regExs << nvidia;
        }

        if ( !info.fAMD )
        {
            regExs << amd;
        }
        auto regEx = regExs.join( "|" );
        terse = removeFromlist( terse, regEx );
        verbose = removeFromlist( verbose, regEx );
    }

    CFFMpegFormats::CFFMpegFormats()
    {
    }

    CFFMpegFormats::CFFMpegFormats( const QString &ffmpegExe )
    {
        setFFMpegExecutable( ffmpegExe );
    }

    void CFFMpegFormats::initEncoderFormatsFromDefaults( const QStringList &terse, const QStringList &verbose, const TFormatMap &formatExtensions )
    {
        fFormats.encoder() = { terse, verbose };
        fMediaEncoderFormatExtensions = formatExtensions;
        computeReverseExtensionMap( true );
        checkLoaded();
        if ( loaded() )
        {
            postLoad();
        }
    }

    void CFFMpegFormats::initDecoderFormatsFromDefaults( const QStringList &terse, const QStringList &verbose, const TFormatMap &formatExtensions )
    {
        fFormats.decoder() = { terse, verbose };
        fMediaDecoderFormatExtensions = formatExtensions;
        computeReverseExtensionMap( false );
        checkLoaded();
        if ( loaded() )
        {
            postLoad();
        }
    }

    void CFFMpegFormats::initVideoEncodingCodecsFromDefaults( const QStringList &terse, const QStringList &verbose )
    {
        fCodecs.encoder().video() = { terse, verbose };

        checkLoaded();
        if ( loaded() )
        {
            postLoad();
        }
    }

    void CFFMpegFormats::initVideoDecodingCodecsFromDefaults( const QStringList &terse, const QStringList &verbose )
    {
        fCodecs.decoder().video() = { terse, verbose };

        checkLoaded();
        if ( loaded() )
        {
            postLoad();
        }
    }

    void CFFMpegFormats::initAudioEncodingCodecsFromDefaults( const QStringList &terse, const QStringList &verbose )
    {
        fCodecs.encoder().audio() = { terse, verbose };

        checkLoaded();
        if ( loaded() )
        {
            postLoad();
        }
    }

    void CFFMpegFormats::initAudioDecodingCodecsFromDefaults( const QStringList &terse, const QStringList &verbose )
    {
        fCodecs.decoder().audio() = { terse, verbose };

        checkLoaded();
        if ( loaded() )
        {
            postLoad();
        }
    }

    void CFFMpegFormats::initSubtitleEncodingCodecsFromDefaults( const QStringList &terse, const QStringList &verbose )
    {
        fCodecs.encoder().subtitle() = { terse, verbose };

        checkLoaded();
        if ( loaded() )
        {
            postLoad();
        }
    }

    void CFFMpegFormats::initSubtitleDecodingCodecsFromDefaults( const QStringList &terse, const QStringList &verbose )
    {
        fCodecs.decoder().subtitle() = { terse, verbose };

        checkLoaded();
        if ( loaded() )
        {
            postLoad();
        }
    }

    void CFFMpegFormats::initVideoEncodersFromDefaults( const QStringList &terse, const QStringList &verbose )
    {
        fEncoderDecoders.encoder().video() = { terse, verbose };

        checkLoaded();
        if ( loaded() )
        {
            postLoad();
        }
    }

    void CFFMpegFormats::initVideoDecodersFromDefaults( const QStringList &terse, const QStringList &verbose )
    {
        fEncoderDecoders.decoder().video() = { terse, verbose };

        checkLoaded();
        if ( loaded() )
        {
            postLoad();
        }
    }

    void CFFMpegFormats::initAudioEncodersFromDefaults( const QStringList &terse, const QStringList &verbose )
    {
        fEncoderDecoders.encoder().audio() = { terse, verbose };

        checkLoaded();
        if ( loaded() )
        {
            postLoad();
        }
    }

    void CFFMpegFormats::initAudioDecodersFromDefaults( const QStringList &terse, const QStringList &verbose )
    {
        fEncoderDecoders.decoder().audio() = { terse, verbose };

        checkLoaded();
        if ( loaded() )
        {
            postLoad();
        }
    }

    void CFFMpegFormats::initSubtitleEncodersFromDefaults( const QStringList &terse, const QStringList &verbose )
    {
        fEncoderDecoders.encoder().subtitle() = { terse, verbose };

        checkLoaded();
        if ( loaded() )
        {
            postLoad();
        }
    }

    void CFFMpegFormats::initSubtitleDecodersFromDefaults( const QStringList &terse, const QStringList &verbose )
    {
        fEncoderDecoders.decoder().subtitle() = { terse, verbose };

        checkLoaded();
        if ( loaded() )
        {
            postLoad();
        }
    }

    void CFFMpegFormats::initHWAccelsFromDefaults( const QStringList &terse, const QStringList &verbose )
    {
        fHWAccels = { terse, verbose };

        checkLoaded();
        if ( loaded() )
        {
            postLoad();
        }
    }

    void CFFMpegFormats::initCodecToEncoderMapDefaults( const TCodecToEncoderDecoderMap &encoderMap )
    {
        fCodecToEncoderMap = encoderMap;
        computeReverseCodecMap( true );
    }

    void CFFMpegFormats::initCodecToDecoderMapDefaults( const TCodecToEncoderDecoderMap &decoderMap )
    {
        fCodecToDecoderMap = decoderMap;
        computeReverseCodecMap( false );
    }

    void CFFMpegFormats::setFFMpegExecutable( const QString &ffmpegExe )
    {
        fFFMpegExe = ffmpegExe;
        Q_ASSERT( validateFFMpegExe() );
        clear();
    }

    void CFFMpegFormats::recompute( QProgressDialog *dlg )
    {
        clear();
        Q_ASSERT( validateFFMpegExe() );
        if ( !validateFFMpegExe() )
            return;

        if ( !dlg->wasCanceled() )
            loadCodecs( dlg );
        if ( !dlg->wasCanceled() )
            loadFormats( dlg );
        if ( !dlg->wasCanceled() )
            loadEncodersDecoders( dlg );
        if ( !dlg->wasCanceled() )
            loadHWAccels( dlg );
        if ( !dlg->wasCanceled() )
            computeReverseExtensionMap( true );
        if ( !dlg->wasCanceled() )
            computeReverseExtensionMap( false );
        if ( !dlg->wasCanceled() )
        {
            fLoaded = true;
            postLoad();
        }
        else
            clear();
    }

    void addAliases( std::set< QString > &retVal, const QString &formatName, const TCodecToEncoderDecoderMap &map )
    {
        for ( auto &&ii : map )
        {
            auto range = ii.second.equal_range( formatName );
            for ( auto kk = range.first; kk != range.second; ++kk )
            {
                retVal.insert( ( *kk ).second );
            }
        }
    }

    std::set< QString > CFFMpegFormats::getCodecAliases( const QString &formatName ) const
    {
        auto pos = fAliases.find( formatName );
        if ( pos == fAliases.end() )
        {
            std::set< QString > retVal;
            retVal.insert( formatName );

            fFormats.addAlias( retVal, formatName );
            fCodecs.addAlias( retVal, formatName );
            fEncoderDecoders.addAlias( retVal, formatName );

            addAliases( retVal, formatName, fCodecToEncoderMap );
            addAliases( retVal, formatName, fCodecToDecoderMap );
            auto pos2 = fEncoderToCodecMap.find( formatName );
            if ( pos2 != fEncoderToCodecMap.end() )
                retVal.insert( ( *pos2 ).second );
            pos2 = fDecoderToCodecMap.find( formatName );
            if ( pos2 != fDecoderToCodecMap.end() )
                retVal.insert( ( *pos2 ).second );

            pos = fAliases.insert( { formatName, retVal } ).first;
        }
        return ( *pos ).second;
    }

    bool isAlias( const std::set< QString > &lhs, const std::set< QString > &rhs )
    {
        std::vector< QString > intersection;
        std::set_intersection( lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::back_inserter( intersection ) );

        return !intersection.empty();
    }

    bool CFFMpegFormats::isCodec( const QString &checkCodecName, const QString &mediaCodecName ) const
    {
        if ( checkCodecName == mediaCodecName )
            return true;
        auto lhsAliases = getCodecAliases( checkCodecName );
        auto rhsAliases = getCodecAliases( mediaCodecName );

        return isAlias( lhsAliases, rhsAliases );
    }

    bool CFFMpegFormats::isContainerFormat( const QString &fileName, const QString &formatName ) const
    {
        auto ext = QFileInfo( fileName ).suffix().toLower();
        auto pos = fExtensionToMediaEncoderFormat.find( ext );
        std::set< QString > aliases;
        if ( pos != fExtensionToMediaEncoderFormat.end() )
        {
            auto curr = getCodecAliases( ( *pos ).second );
            aliases.insert( curr.begin(), curr.end() );
        }
        pos = fExtensionToMediaDecoderFormat.find( ext );
        if ( pos != fExtensionToMediaDecoderFormat.end() )
        {
            auto curr = getCodecAliases( ( *pos ).second );
            aliases.insert( curr.begin(), curr.end() );
        }
        return isAlias( { formatName }, aliases );
    }

    bool CFFMpegFormats::isEncoderFormat( const QString &suffix, const QString &formatName ) const
    {
        Q_ASSERT( loaded() );
        auto realSuffix = suffix;
        if ( !realSuffix.startsWith( "*." ) )
            realSuffix = "*." + realSuffix;
        return getEncoderExtensionsForFormat( formatName ).contains( realSuffix );
    }

    bool CFFMpegFormats::isDecoderFormat( const QString &suffix, const QString &formatName ) const
    {
        Q_ASSERT( loaded() );
        auto realSuffix = suffix;
        if ( !realSuffix.startsWith( "*." ) )
            realSuffix = "*." + realSuffix;
        return getDecoderExtensionsForFormat( formatName ).contains( realSuffix );
    }

    QStringList CFFMpegFormats::encoderFormats( bool verbose ) const
    {
        return fFormats.encoder().data( verbose );
    }

    QStringList CFFMpegFormats::decoderFormats( bool verbose ) const
    {
        return fFormats.encoder().data( verbose );
    }

    QStringList CFFMpegFormats::audioEncodingCodecs( bool verbose ) const
    {
        return fCodecs.encoder().audio().data( verbose );
    }

    QStringList CFFMpegFormats::audioDecodingCodecs( bool verbose ) const
    {
        return fCodecs.decoder().audio().data( verbose );
    }

    QStringList CFFMpegFormats::videoEncodingCodecs( bool verbose ) const
    {
        return fCodecs.encoder().video().data( verbose );
    }

    QStringList CFFMpegFormats::videoDecodingCodecs( bool verbose ) const
    {
        return fCodecs.decoder().video().data( verbose );
    }

    QStringList CFFMpegFormats::subtitleEncodingCodecs( bool verbose ) const
    {
        return fCodecs.encoder().subtitle().data( verbose );
    }

    QStringList CFFMpegFormats::subtitleDecodingCodecs( bool verbose ) const
    {
        return fCodecs.decoder().subtitle().data( verbose );
    }

    QStringList CFFMpegFormats::audioEncoders( bool verbose ) const
    {
        return fEncoderDecoders.encoder().audio().data( verbose );
    }

    QStringList CFFMpegFormats::audioDecoders( bool verbose ) const
    {
        return fEncoderDecoders.decoder().audio().data( verbose );
    }

    QStringList CFFMpegFormats::videoEncoders( bool verbose ) const
    {
        return fEncoderDecoders.encoder().video().data( verbose );
    }

    QStringList CFFMpegFormats::videoDecoders( bool verbose ) const
    {
        return fEncoderDecoders.decoder().video().data( verbose );
    }

    QStringList CFFMpegFormats::subtitleEncoders( bool verbose ) const
    {
        return fEncoderDecoders.encoder().subtitle().data( verbose );
    }

    QStringList CFFMpegFormats::subtitleDecoders( bool verbose ) const
    {
        return fEncoderDecoders.decoder().subtitle().data( verbose );
    }

    QStringList CFFMpegFormats::hwAccels( bool verbose ) const
    {
        return fHWAccels.data( verbose );
    }

    void CFFMpegFormats::checkLoaded()
    {
        bool loaded = fFormats.isLoaded() /* && !fMediaFormatExtensions.empty() && !fReverseMediaFormatExtensions.empty() */;

        loaded = loaded && fCodecs.isLoaded();
        loaded = loaded && fEncoderDecoders.isLoaded();
        loaded = loaded && fHWAccels.isLoaded();

        fLoaded = loaded;
    }

    void CFFMpegFormats::clear()
    {
        fLoaded = false;

        fFormats.clear();

        fMediaEncoderFormatExtensions.clear();
        fExtensionToMediaEncoderFormat.clear();

        fMediaDecoderFormatExtensions.clear();
        fExtensionToMediaDecoderFormat.clear();

        fCodecToEncoderMap.clear();
        fEncoderToCodecMap.clear();
        fCodecToDecoderMap.clear();
        fDecoderToCodecMap.clear();

        fCodecs.clear();
        fEncoderDecoders.clear();

        fHWAccels.clear();

        fImageFormats.reset();
    }

    bool CFFMpegFormats::validateFFMpegExe() const
    {
        return validateFFMpegExe( fFFMpegExe );
    }

    bool CFFMpegFormats::validateFFMpegExe( const QString &ffmpegExe )
    {
        if ( ffmpegExe.isEmpty() )
            return false;
        auto fi = QFileInfo( ffmpegExe );
        bool aOK = fi.exists() && fi.isExecutable();
        return aOK;
    }

    bool CFFMpegFormats::validate() const
    {
        bool aOK = loaded();
        aOK = fFormats.validate() && aOK;

        aOK = fCodecs.validate() && aOK;
        aOK = fEncoderDecoders.validate() && aOK;

        aOK = fHWAccels.validate() && aOK;
        return aOK;
    }

    void CFFMpegFormats::postLoad()
    {
        Q_ASSERT( validate() );
        fFormats.sort();
        fCodecs.sort();
        fEncoderDecoders.sort();
        fHWAccels.sort();
        Q_ASSERT( validate() );
    }

    QStringList CFFMpegFormats::getExtensions( const TFormatMap &map, EFormatType extensionType, const QStringList &exclude ) const
    {
        auto pos = map.find( extensionType );
        if ( pos == map.end() )
            return {};

        QStringList retVal;
        for ( auto &&ii : ( *pos ).second )
        {
            retVal << ii.second;
        }

        retVal.removeDuplicates();
        return std::move( removeFromlist( retVal, exclude ) );
    }

    QStringList CFFMpegFormats::getEncoderExtensions( EFormatType extensionType, const QStringList &exclude ) const
    {
        return getExtensions( fMediaEncoderFormatExtensions, extensionType, exclude );
    }

    QStringList CFFMpegFormats::getDecoderExtensions( EFormatType extensionType, const QStringList &exclude ) const
    {
        return getExtensions( fMediaDecoderFormatExtensions, extensionType, exclude );
    }

    QStringList CFFMpegFormats::getVideoExtensions( const QStringList &exclude /*= {} */ ) const
    {
        auto retVal = QStringList() << getVideoDecoderExtensions( exclude ) << getVideoEncoderExtensions( exclude );
        retVal.removeDuplicates();
        return retVal;
    }

    QStringList CFFMpegFormats::getAudioExtensions( const QStringList &exclude /*= {} */ ) const
    {
        auto retVal = QStringList() << getAudioDecoderExtensions( exclude ) << getAudioEncoderExtensions( exclude );
        retVal.removeDuplicates();
        return retVal;
    }

    QStringList CFFMpegFormats::getSubtitleExtensions( const QStringList &exclude /*= {} */ ) const
    {
        auto retVal = QStringList() << getSubtitleEncoderExtensions( exclude ) << getSubtitleDecoderExtensions( exclude );
        retVal.removeDuplicates();
        return retVal;
    }

    QStringList CFFMpegFormats::getVideoEncoderExtensions( const QStringList &exclude ) const
    {
        return getEncoderExtensions( EFormatType::eVideo, exclude );
    }

    QStringList CFFMpegFormats::getAudioEncoderExtensions( const QStringList &exclude ) const
    {
        return getEncoderExtensions( EFormatType::eAudio, exclude );
    }

    QStringList CFFMpegFormats::getSubtitleEncoderExtensions( const QStringList &exclude ) const
    {
        return getEncoderExtensions( EFormatType::eSubtitle, exclude );
    }

    QStringList CFFMpegFormats::getVideoDecoderExtensions( const QStringList &exclude ) const
    {
        return getDecoderExtensions( EFormatType::eVideo, exclude );
    }

    QStringList CFFMpegFormats::getAudioDecoderExtensions( const QStringList &exclude ) const
    {
        return getDecoderExtensions( EFormatType::eAudio, exclude );
    }

    QStringList CFFMpegFormats::getSubtitleDecoderExtensions( const QStringList &exclude ) const
    {
        return getDecoderExtensions( EFormatType::eSubtitle, exclude );
    }

    QStringList CFFMpegFormats::getImageEncoderExtensions( const QStringList &exclude ) const
    {
        return getEncoderExtensions( EFormatType::eImage, exclude );
    }

    QStringList CFFMpegFormats::getImageDecoderExtensions( const QStringList &exclude ) const
    {
        return getDecoderExtensions( EFormatType::eImage, exclude );
    }

    QString CFFMpegFormats::getPrimaryEncoderExtensionForFormat( const QString &formatName, const QStringList &exclude ) const
    {
        auto exts = getEncoderExtensionsForFormat( formatName, exclude );
        if ( exts.isEmpty() )
            return {};
        auto retVal = exts.front();
        retVal.remove( "*." );
        return retVal;
    }

    QString CFFMpegFormats::getPrimaryDecoderExtensionForFormat( const QString &formatName, const QStringList &exclude ) const
    {
        auto exts = getDecoderExtensionsForFormat( formatName, exclude );
        if ( exts.isEmpty() )
            return {};
        auto retVal = exts.front();
        retVal.remove( "*." );
        return retVal;
    }

    std::optional< QStringList > CFFMpegFormats::encoderFormatLoaded( const QString &formatName ) const
    {
        for ( auto &&ii : fMediaEncoderFormatExtensions )
        {
            auto pos = ii.second.find( formatName );
            if ( pos == ii.second.end() )
                return {};
            return ( *pos ).second;
        }
        return {};
    }

    std::optional< QStringList > CFFMpegFormats::decoderFormatLoaded( const QString &formatName ) const
    {
        for ( auto &&ii : fMediaDecoderFormatExtensions )
        {
            auto pos = ii.second.find( formatName );
            if ( pos == ii.second.end() )
                return {};
            return ( *pos ).second;
        }
        return {};
    }

    QStringList CFFMpegFormats::getExtensionsForFormat( const TFormatMap &map, const QString &formatName, const QStringList &exclude ) const
    {
        Q_ASSERT( loaded() );
        for ( auto &&ii : map )
        {
            auto pos = ii.second.find( formatName );
            if ( pos == ii.second.end() )
                continue;

            return std::move( removeFromlist( ( *pos ).second, exclude ) );
        }
        return {};
    }

    QStringList CFFMpegFormats::getEncoderExtensionsForFormat( const QString &formatName, const QStringList &exclude ) const
    {
        return getExtensionsForFormat( fMediaEncoderFormatExtensions, formatName, exclude );
    }

    QStringList CFFMpegFormats::getDecoderExtensionsForFormat( const QString &formatName, const QStringList &exclude ) const
    {
        return getExtensionsForFormat( fMediaDecoderFormatExtensions, formatName, exclude );
    }

    QString CFFMpegFormats::getTranscodeHWAccel( const QString &formatName ) const
    {
        if ( formatName.contains( "_qsv" ) )
            return "qsv";
        if ( formatName.contains( "_nvenc" ) )
            return "cuda";
        if ( formatName.contains( "_amf" ) )
            return "amd";
        return {};
    }

    QString CFFMpegFormats::getCodecForHWAccel( const QString &hwAccel ) const
    {
        if ( hwAccel.contains( "cuda" ) )
            return "hevc_nvenc";
        if ( hwAccel.contains( "qsv" ) )
            return "hevc_qsv";
        if ( hwAccel.contains( "amd" ) )
            return "hevc_amf";
        return {};
    }

    void CFFMpegFormats::loadCodecs( QProgressDialog *dlg )
    {
        if ( !validateFFMpegExe() )
            return;

        if ( dlg )
            dlg->setLabelText( QStringLiteral( "Loading Codecs" ) );

        QProcess process;
        process.start(
            fFFMpegExe, QStringList() << "-hide_banner"
                                      << "-codecs" );

        process.waitForFinished();
        auto codecs = process.readAllStandardOutput();

        auto pos = codecs.indexOf( "-------" );
        if ( pos == -1 )
            return;

        codecs = codecs.mid( pos + 7 );
        /*
          Codecs:
           D..... = Decoding supported
           .E.... = Encoding supported
           ..V... = Video codec
           ..A... = Audio codec
           ..S... = Subtitle codec
           ..D... = Data codec
           ..T... = Attachment codec
           ...I.. = Intra frame-only codec
           ....L. = Lossy compression
           .....S = Lossless compression
           -------
           D.VI.S 012v                 Uncompressed 4:2:2 10-bit
           D.V.L. 4xm                  4X Movie
           D.VI.S 8bps                 QuickTime 8BPS video
        */

        auto regEx = QRegularExpression( R"((?<decoder>D|\.)(?<encoder>E|\.)(?<type>[VASDT])(?<intraframeonly>I|\.)(?<lossy>L|\.)(?<lossless>S|\.)\s+(?<name>\S+)\s*(?<desc>[^\r\n].+))" );
        auto ii = regEx.globalMatch( QString::fromUtf8( codecs ) );
        int count = 0;
        if ( dlg )
        {
            while ( ii.hasNext() )
            {
                count++;
                ii.next();
            }
            ii = regEx.globalMatch( QString::fromUtf8( codecs ) );
            dlg->setRange( 0, count );
            dlg->setValue( 0 );
        }

        while ( ii.hasNext() )
        {
            if ( dlg )
            {
                dlg->setValue( dlg->value() + 1 );
                if ( dlg->wasCanceled() )
                    break;
            }
            auto match = ii.next();
            auto isDecoder = match.captured( "decoder" ) != '.';
            auto isEncoder = match.captured( "encoder" ) != '.';
            auto type = match.captured( "type" );
            if ( ( type[ 0 ] != 'V' ) && ( type[ 0 ] != 'A' ) && ( type[ 0 ] != 'S' ) )
                continue;
            auto intraframeonly = match.captured( "intraframeonly" ) != '.';
            auto lossy = match.captured( "lossy" ) != '.';
            auto lossless = match.captured( "lossless" ) != '.';
            auto name = match.captured( "name" ).trimmed();
            auto desc = match.captured( "desc" ).trimmed();

            if ( dlg )
                dlg->setLabelText( QStringLiteral( "Loading Codec: %1" ).arg( name ) );

            auto objectsRegExp = QRegularExpression( R"((\((?<codertype>encoders|decoders)\:\s*(?<codingCodecs>[^\)\r\n]+)\s*\)\s*))" );

            auto jj = objectsRegExp.globalMatch( desc );
            QStringList encodingCodecs;
            QStringList decodingCodecs;
            if ( jj.hasNext() )
            {
                auto tmp = jj;
                desc = desc.left( tmp.next().capturedStart() ).trimmed();
            }
            while ( jj.hasNext() )
            {
                auto currMatch = jj.next();
                auto type = currMatch.captured( "codertype" );
                if ( type == "encoders" )
                    encodingCodecs = currMatch.captured( "codingCodecs" ).split( ' ', Qt::SkipEmptyParts );
                else
                    decodingCodecs = currMatch.captured( "codingCodecs" ).split( ' ', Qt::SkipEmptyParts );
            }

            QStringList tmp;
            if ( intraframeonly )
                tmp << "(Intra frame-only codec)";
            if ( lossy )
                tmp << "(Lossy compression)";
            if ( lossless )
                tmp << "(Lossless compression)";

            desc = name + " - " + desc;
            if ( !tmp.isEmpty() )
                desc += " - " + tmp.join( " " );

            auto formatType = EFormatType::eUnknown;
            switch ( type[ 0 ].toLatin1() )
            {
                case 'A':
                    {
                        formatType = EFormatType::eAudio;
                        if ( isEncoder )
                            fCodecs.encoder().audio().push_back( name, desc );

                        if ( isDecoder )
                            fCodecs.decoder().audio().push_back( name, desc );
                    }
                    break;
                case 'V':
                    {
                        formatType = EFormatType::eVideo;
                        if ( isEncoder )
                        {
                            fCodecs.encoder().video().push_back( name, desc );
                        }
                        if ( isDecoder )
                        {
                            fCodecs.decoder().video().push_back( name, desc );
                        }
                    }
                    break;
                case 'S':
                    {
                        formatType = EFormatType::eSubtitle;
                        if ( isEncoder )
                        {
                            fCodecs.encoder().subtitle().push_back( name, desc );
                        }
                        if ( isDecoder )
                        {
                            fCodecs.decoder().subtitle().push_back( name, desc );
                        }
                    }
                    break;
                default:
                    break;
            }
            if ( formatType == EFormatType::eUnknown )
                continue;

            if ( isEncoder )
            {
                for ( auto &&ii : encodingCodecs )
                {
                    fCodecToEncoderMap[ formatType ].insert( { name, ii } );
                    fEncoderToCodecMap[ ii ] = name;
                }
                computeExtensionsForFormat( name, desc, formatType, true );
            }

            if ( isDecoder )
            {
                for ( auto &&ii : decodingCodecs )
                {
                    fCodecToDecoderMap[ formatType ].insert( { name, ii } );
                    fDecoderToCodecMap[ ii ] = name;
                }
                computeExtensionsForFormat( name, desc, formatType, false );
            }
        }
    }

    void CFFMpegFormats::loadEncodersDecoders( QProgressDialog *dlg )
    {
        loadEncodersDecoders( true, dlg );
        if ( !dlg->wasCanceled() )
            loadEncodersDecoders( false, dlg );
    }

    void CFFMpegFormats::loadEncodersDecoders( bool isEncoding, QProgressDialog *dlg )
    {
        if ( !validateFFMpegExe() )
            return;

        if ( dlg )
            dlg->setLabelText( QStringLiteral( "Loading %1" ).arg( isEncoding ? "Encoding Codecs" : "Decoding Codecs" ) );

        QProcess process;
        process.start( fFFMpegExe, QStringList() << "-hide_banner" << ( isEncoding ? "-encoders" : "-decoders" ) );
        process.waitForFinished();
        auto codecs = process.readAllStandardOutput();

        auto pos = codecs.indexOf( "------" );
        if ( pos == -1 )
            return;

        codecs = codecs.mid( pos + 6 );
        /*
        *EncodingCodecs/DecodingCodecs:
            V..... = Video
            A..... = Audio
            S..... = Subtitle
        .   F.... = Frame-level multithreading
            .S... = Slice-level multithreading
            ...  X.. = Codec is experimental
        ....   B. = Supports draw_horiz_band
        .....   D = Supports direct rendering method 1
                    
                    
            V....D a64multi             Multicolor charset for Commodore 64 (codec a64_multi)
    */
        auto regEx = QRegularExpression( R"((?<type>[VAS][FSXBD\.]{5})\s+(?<name>\S+)\s+(?<desc>.*))" );
        auto ii = regEx.globalMatch( QString::fromUtf8( codecs ) );
        int count = 0;
        if ( dlg )
        {
            while ( ii.hasNext() )
            {
                count++;
                ii.next();
            }
            ii = regEx.globalMatch( QString::fromUtf8( codecs ) );
            dlg->setRange( 0, count );
            dlg->setValue( 0 );
        }

        while ( ii.hasNext() )
        {
            if ( dlg )
            {
                dlg->setValue( dlg->value() + 1 );
                if ( dlg->wasCanceled() )
                    break;
            }

            auto match = ii.next();
            auto type = match.captured( "type" );
            if ( type.length() != 6 )
                continue;
            if ( type[ 3 ] == 'X' )
                continue;
            if ( ( type[ 0 ] != 'V' ) && ( type[ 0 ] != 'A' ) && ( type[ 0 ] != 'S' ) )
                continue;
            auto name = match.captured( "name" ).trimmed();
            auto desc = name + " - " + match.captured( "desc" ).trimmed();

            if ( dlg )
                dlg->setLabelText( QStringLiteral( "Loading %1: %2" ).arg( isEncoding ? "Encoding Codec" : "Decoding Codec" ).arg( name ) );

            switch ( type[ 0 ].toLatin1() )
            {
                case 'A':
                    if ( isEncoding )
                    {
                        fEncoderDecoders.encoder().audio().push_back( name, desc );
                    }
                    else
                    {
                        fEncoderDecoders.decoder().audio().push_back( name, desc );
                    }
                    break;
                case 'V':
                    if ( isEncoding )
                    {
                        fEncoderDecoders.encoder().video().push_back( name, desc );
                    }
                    else
                    {
                        fEncoderDecoders.decoder().video().push_back( name, desc );
                    }
                    break;
                case 'S':
                    if ( isEncoding )
                    {
                        fEncoderDecoders.encoder().subtitle().push_back( name, desc );
                    }
                    else
                    {
                        fEncoderDecoders.decoder().subtitle().push_back( name, desc );
                    }
                    break;
                default:
                    break;
            }
        }
    }

    void CFFMpegFormats::loadHWAccels( QProgressDialog *dlg )
    {
        if ( !validateFFMpegExe() )
            return;

        if ( dlg )
            dlg->setLabelText( "Loading HW Accelerators" );

        QProcess process;
        process.start(
            fFFMpegExe, QStringList() << "-hide_banner"
                                      << "-hwaccels" );
        process.waitForFinished();
        auto hwaccels = process.readAllStandardOutput();

        auto pos = hwaccels.indexOf( ":" );
        if ( pos == -1 )
            return;

        hwaccels = hwaccels.mid( pos + 1 );
        /*
            * Hardware acceleration methods:
            cuda
            dxva2
            qsv
            d3d11va
        */
        auto regEx = QRegularExpression( R"(\s*(?<hwaccel>\S+))" );
        auto ii = regEx.globalMatch( QString::fromUtf8( hwaccels ) );
        int count = 0;
        if ( dlg )
        {
            while ( ii.hasNext() )
            {
                count++;
                ii.next();
            }
            ii = regEx.globalMatch( QString::fromUtf8( hwaccels ) );
            dlg->setRange( 0, count );
            dlg->setValue( 0 );
        }

        while ( ii.hasNext() )
        {
            if ( dlg )
            {
                dlg->setValue( dlg->value() + 1 );
                if ( dlg->wasCanceled() )
                    break;
            }
            auto match = ii.next();
            auto type = match.captured( "hwaccel" ).trimmed().toLower();
            if ( dlg )
                dlg->setLabelText( QStringLiteral( "Loading HW Accelerator: %1" ).arg( type ) );

            QString verbose;
            if ( type == "none" )
                verbose = type + " - Do not use any hardware acceleration (the default).";
            else if ( type == "auto" )
                verbose = type + " - Automatically select the hardware acceleration method.";
            else if ( type == "vdpau" )
                verbose = type + " - Use VDPAU (Video Decode and Presentation API for Unix) hardware acceleration.";
            else if ( type == "dxva2" )
                verbose = type + " - Use DXVA2 (DirectX Video Acceleration) API hardware acceleration.";
            else if ( type == "d3d11va" )
                verbose = type + " - Use D3D11VA (DirectX Video Acceleration) API hardware acceleration.";
            else if ( type == "vaapi" )
                verbose = type + " - Use VAAPI (Video Acceleration API) hardware acceleration.";
            else if ( type == "qsv" )
                verbose = type + " - Use the Intel QuickSync Video acceleration for video transcoding.";
            else if ( type == "cuda" )
                verbose = type + " - Use the nVidia Cuda Video acceleration for video transcoding.";
            else if ( type == "amf" )
                verbose = type + " - Use the AMD Advanced Media Framework Video acceleration for video transcoding.";
            else if ( type == "opencl" )
                verbose = type + " - Use OpenCL hardware acceleration for API video transcoding.)";   //
            else if ( type == "vulkan" )
                verbose = type + " - Use the Vulkan hardware acceleration API for video transcoding.)";   //
            else
                continue;
            fHWAccels.push_back( type, verbose );
        }
    }

    void CFFMpegFormats::loadFormats( QProgressDialog *dlg )
    {
        if ( !validateFFMpegExe() )
            return;

        if ( dlg )
            dlg->setLabelText( QObject::tr( "Loading Formats" ) );

        QProcess process;
        process.start(
            fFFMpegExe, QStringList() << "-hide_banner"
                                      << "-formats" );
        process.waitForFinished();
        auto formats = process.readAllStandardOutput();

        auto pos = formats.indexOf( "--" );
        if ( pos == -1 )
            return;

        formats = formats.mid( pos + 2 );
        /*
        * Encoers/DecodingCodecs:
            D. = Demuxing supported
            .E = Muxing supported
                        
            D  3dostr          3DO STR
            E 3g2             3GP2 (3GPP2 file format)
        */

        auto regEx = QRegularExpression( R"((?<type>[DE]{1,2})\s+(?<name>\S+)\s+(?<desc>.*))" );
        auto ii = regEx.globalMatch( QString::fromUtf8( formats ) );
        int count = 0;
        if ( dlg )
        {
            while ( ii.hasNext() )
            {
                count++;
                ii.next();
            }
            ii = regEx.globalMatch( QString::fromUtf8( formats ) );
            dlg->setRange( 0, count );
            dlg->setValue( 0 );
        }

        while ( ii.hasNext() )
        {
            if ( dlg )
            {
                dlg->setValue( dlg->value() + 1 );
                if ( dlg->wasCanceled() )
                    break;
            }
            auto match = ii.next();
            auto type = match.captured( "type" ).trimmed();
            bool isEncoder = type.contains( 'E' );
            bool isDecoder = type.contains( 'D' );
            if ( !isEncoder && !isDecoder )
                continue;
            auto names = match.captured( "name" ).trimmed().split( "," );
            for ( auto &&ii : names )
                ii = ii.trimmed();
            auto name = names.join( "," ).trimmed();
            auto desc = match.captured( "desc" ).trimmed();

            if ( dlg )
                dlg->setLabelText( QObject::tr( "Loading Format: %1" ).arg( name ) );

            if ( isEncoder )
                computeExtensionsForFormat( name, desc, {}, true );
            if ( isDecoder )
                computeExtensionsForFormat( name, desc, {}, false );
        }
    }

    void CFFMpegFormats::computeExtensionsForFormat( const QString &name, const QString &desc, std::optional< EFormatType > formatType, bool isEncoder )
    {
        auto exts = computeExtensionsForFormat( name, formatType, isEncoder );
        if ( exts.empty() )
            return;

        auto verbose = name + " - " + desc + " (" + exts.join( ";" ) + ")";
        if ( isEncoder )
        {
            fFormats.encoder().push_back( name, verbose );
        }
        else
        {
            fFormats.decoder().push_back( name, verbose );
        }
    }

    QStringList CFFMpegFormats::computeExtensionsForFormat( const QString &formatName, std::optional< EFormatType > formatType, bool isEncoder )
    {
        if ( !validateFFMpegExe() )
            return {};

        auto retVal = isEncoder ? encoderFormatLoaded( formatName ) : decoderFormatLoaded( formatName );
        if ( !retVal.has_value() )
        {
            retVal = QStringList();
            QProcess process;
            process.start(
                fFFMpegExe, QStringList() << "-hide_banner"
                                          << "-h" << ( isEncoder ? QStringLiteral( "muxer=" ) : QStringLiteral( "demuxer=" ) ) + formatName );
            process.waitForFinished();
            auto formatHelp = process.readAllStandardOutput();

            // Common extensions: 3g2.
            auto regEx = QRegularExpression( R"(Common extensions\:\s*(?<exts>.*)\.)" );
            auto match = regEx.match( QString::fromUtf8( formatHelp ) );
            if ( match.hasMatch() )
            {
                auto tmp = match.captured( "exts" ).trimmed();
                retVal = match.captured( "exts" ).trimmed().split( "," );
                for ( auto &&ii : retVal.value() )
                    ii = "*." + ii.trimmed().toLower();
                retVal.value().removeDuplicates();
            }
            else
                return retVal.value();

            std::list< EFormatType > formats;
            auto lc = formatName.toLower();
            if ( ( lc == "image2" ) || ( lc == "webp" ) )
                formats.push_back( EFormatType::eImage );
            else
            {
                // Mime type: video/x-matroska.
                regEx = QRegularExpression( R"(Mime type\:\s*(?<mimetype>.*)\.)" );
                match = regEx.match( QString::fromUtf8( formatHelp ) );
                if ( match.hasMatch() )
                {
                    auto mime = match.captured( "mimetype" ).trimmed().toLower();
                    if ( mime.contains( "video" ) )
                        formats.push_back( EFormatType::eVideo );
                    if ( mime.contains( "audio" ) )
                        formats.push_back( EFormatType::eAudio );
                    if ( mime.contains( "subtitle" ) )
                        formats.push_back( EFormatType::eSubtitle );
                    if ( mime.contains( "image" ) )
                        formats.push_back( EFormatType::eImage );
                }

                if ( formats.empty() )
                {
                    regEx = QRegularExpression( R"(Default\s+(?<codec>.*)\s+codec\:)" );
                    auto ii = regEx.globalMatch( QString::fromUtf8( formatHelp ) );

                    while ( ii.hasNext() )
                    {
                        auto match = ii.next();

                        auto codec = match.captured( "codec" ).toLower().trimmed();
                        if ( codec == "video" )
                            formats.push_back( EFormatType::eVideo );
                        else if ( codec == "audio" )
                            formats.push_back( EFormatType::eAudio );
                        else if ( codec == "subtitle" )
                            formats.push_back( EFormatType::eSubtitle );
                        // images will return video codec
                    }
                }
            }
            if ( formats.empty() && formatType.has_value() )
            {
                formats.push_back( formatType.value() );
            }
            for ( auto &&format : formats )
            {
                if ( isEncoder )
                    fMediaEncoderFormatExtensions[ format ][ formatName ] = retVal.value();
                else
                    fMediaDecoderFormatExtensions[ format ][ formatName ] = retVal.value();
            }
        }
        return retVal.value();
    }

    bool CFFMpegFormats::isImageFormat( const QString &ext ) const
    {
        if ( !fImageFormats.has_value() )
        {
            fImageFormats = std::unordered_set< QString >();
            auto tmp = QImageReader::supportedImageFormats();
            for ( auto &&ii : tmp )
            {
                fImageFormats.value().insert( QStringLiteral( "*." ) + QString::fromUtf8( ii ) );
                fImageFormats.value().insert( QString::fromUtf8( ii ) );
            }
        }

        return ( fImageFormats.value().find( ext ) != fImageFormats.value().end() );
    }

    void CFFMpegFormats::computeReverseCodecMap( bool encodingCodecs )
    {
        auto &&whichReverseMap = encodingCodecs ? fEncoderToCodecMap : fDecoderToCodecMap;
        auto &&whichCodecs = encodingCodecs ? fCodecToEncoderMap : fCodecToDecoderMap;

        whichReverseMap.clear();

        for ( auto &&ii : whichCodecs )
        {
            for ( auto &&jj : ii.second )
            {
                for ( auto &&kk : jj.second )
                {
                    if ( whichReverseMap.find( kk ) == whichReverseMap.end() )
                        whichReverseMap[ kk ] = jj.first;
                }
            }
        }
    }

    void CFFMpegFormats::computeReverseExtensionMap( bool encodingCodecs )
    {
        auto &&whichReverseMap = encodingCodecs ? fExtensionToMediaEncoderFormat : fExtensionToMediaDecoderFormat;
        auto &&whichExtensions = encodingCodecs ? fMediaEncoderFormatExtensions : fMediaDecoderFormatExtensions;

        whichReverseMap.clear();

        for ( auto &&ii : whichExtensions )
        {
            for ( auto &&jj : ii.second )
            {
                for ( auto &&kk : jj.second )
                {
                    if ( whichReverseMap.find( kk ) == whichReverseMap.end() )
                        whichReverseMap[ kk ] = jj.first;
                }
            }
        }
    }

    QString toString( EFormatType type )
    {
        switch ( type )
        {
            case EFormatType::eUnknown:
                return "NTowel42MediaUtils::EFormatType::eUnknown";
            case EFormatType::eVideo:
                return "NTowel42MediaUtils::EFormatType::eVideo";
            case EFormatType::eAudio:
                return "NTowel42MediaUtils::EFormatType::eAudio";
            case EFormatType::eSubtitle:
                return "NTowel42MediaUtils::EFormatType::eSubtitle";
            case EFormatType::eImage:
                return "NTowel42MediaUtils::EFormatType::eImage";
        }
        return toString( EFormatType::eUnknown );
    }

    CFFMpegFormats::SStringListPair::SStringListPair( const std::initializer_list< QStringList > &data )
    {
        auto ii = data.begin();
        if ( ii != data.end() )
        {
            fTerse = *ii;
            ii++;
        }
        if ( ii != data.end() )
            fVerbose = *ii;

#ifdef _DEBUG
        for ( auto &&ii : fTerse )
            fExistingTerse.insert( ii );

        auto t1 = fTerse;
        t1.removeDuplicates();

        auto t2 = fVerbose;
        t2.removeDuplicates();

        Q_ASSERT( fExistingTerse.size() == fTerse.count() );
        Q_ASSERT( t1.count() == fTerse.count() );
        Q_ASSERT( t2.count() == fVerbose.count() );
#endif
    }

    void CFFMpegFormats::SStringListPair::clear()
    {
        fTerse.clear();
        fVerbose.clear();
#ifdef _DEBUG
        fExistingTerse.clear();
#endif
    }

    void CFFMpegFormats::SStringListPair::addAlias( std::set< QString > &retVal, const QString &name ) const
    {
        auto pos = fTerse.indexOf( name );
        if ( pos != -1 )
            retVal.insert( fVerbose[ pos ] );
        pos = fVerbose.indexOf( name );
        if ( pos != -1 )
            retVal.insert( fTerse[ pos ] );
    }

    bool CFFMpegFormats::SStringListPair::validate() const
    {
        return fTerse.count() == fVerbose.count();
    }

    void CFFMpegFormats::SStringListPair::sort()
    {
        Q_ASSERT( validate() );
        if ( !validate() )
            return;

        std::map< QString, QString > tmp;
        for ( int ii = 0; ii < fTerse.count(); ++ii )
        {
            auto &&terse = fTerse[ ii ];
            auto &&verbose = fVerbose[ ii ];
            Q_ASSERT( tmp.find( terse ) == tmp.end() );
            tmp[ terse ] = verbose;
        }
        Q_ASSERT( tmp.size() == fTerse.count() );
        fTerse.clear();
        fVerbose.clear();
        for ( auto &&ii : tmp )
        {
            fTerse.push_back( ii.first );
            fVerbose.push_back( ii.second );
        }
    }

    void CFFMpegFormats::SStringListPair::push_back( const QString &terse, const QString &verbose )
    {
        if ( fExistingTerse.find( terse ) != fExistingTerse.end() )
            return;
        fExistingTerse.insert( terse );
        fTerse.push_back( terse );
        fVerbose.push_back( verbose );
    }

    void CFFMpegFormats::SVideoAudioSubtitle::addAlias( std::set< QString > &retVal, const QString &name ) const
    {
        audio().addAlias( retVal, name );
        video().addAlias( retVal, name );
        subtitle().addAlias( retVal, name );
    }

    bool CFFMpegFormats::SVideoAudioSubtitle::isLoaded() const
    {
        return video().isLoaded() && audio().isLoaded() && subtitle().isLoaded();
    }

    void CFFMpegFormats::SVideoAudioSubtitle::clear()
    {
        video().clear();
        audio().clear();
        subtitle().clear();
    }

    bool CFFMpegFormats::SVideoAudioSubtitle::validate() const
    {
        return video().validate() && audio().validate() && subtitle().validate();
    }

    void CFFMpegFormats::SVideoAudioSubtitle::sort()
    {
        video().sort();
        audio().sort();
        subtitle().sort();
    }
}
