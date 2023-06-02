<script lang="ts">
	import ContourPlot  from '../lib/plots/ContourPlot.svelte'
	import TimelapseVideo  from '../lib/TimelapseVideo.svelte'
</script>

<kor-grid>
	<kor-card grid-cols="6" grid-rows="1">
		<ContourPlot />
	</kor-card>
	<kor-card label="" grid-cols="6" grid-rows="2">
		<TimelapseVideo />
	</kor-card>
	<kor-card  label="" grid-cols="6" grid-rows="1">
		<ContourPlot />
	</kor-card>
</kor-grid>