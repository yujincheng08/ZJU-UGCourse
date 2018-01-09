<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
xmlns:xi="http://www.w3.org/2004/XInclude">
<xsl:template match="//root/project">
<html>
    <head>
    <title> <xsl:value-of select="@name"/> </title>
    <link rel="stylesheet" type="text/css" href="project.css" />
    </head>
    <body>
        <h1 class="projectname"><xsl:value-of select="@name"/></h1>
        <xsl:apply-templates select="abstract"/>
        <table class="members">
            <tr>
                <th>Position</th>
                <th>Name</th>
                <th>Student id</th>
                <th>Gender</th>
                <th>Duty</th>
            </tr>
            <xsl:for-each select="*/person">
                <xsl:apply-templates select="."/>
            </xsl:for-each>
        </table>
    </body>
</html>
</xsl:template>

<xsl:template match="abstract">
<p>
    <xsl:attribute name="class">
        <xsl:value-of select="name(.)"/>
    </xsl:attribute>
    <xsl:value-of select="."/>
</p>
</xsl:template>

<xsl:template match="person">
<tr>
    <xsl:attribute name="class">
    <xsl:value-of select="name(.)"/>
    </xsl:attribute>
    <td><xsl:value-of select="name(parent::*)"/></td>
    <td><xsl:value-of select="name"/></td>
    <td><xsl:value-of select="stuid"/></td>
    <td><xsl:value-of select="name(gender/*)"/></td>
    <td><xsl:value-of select="duty"/></td>
</tr>
</xsl:template>

</xsl:stylesheet>