<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:fo="http://www.w3.org/1999/XSL/Format"
    xmlns:xse_s="http://terminus.romhack.net/XmlScriptEditor_playlet">
    <xsl:output method="text" encoding="UTF-8" />
    <xsl:strip-space elements="*"/>
    <xsl:template match="/xse_s:dump/xse_s:playlet">
        <xsl:for-each select="xse_s:animation">
            <xsl:value-of select="@length" />
            <xsl:text>&#xA;</xsl:text>
        </xsl:for-each>
    </xsl:template>

</xsl:stylesheet>
